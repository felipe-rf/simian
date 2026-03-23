# Simian DNA API

A C++17 REST API built with Crow to detect simian DNA sequences and expose aggregate stats.

## Features

- `POST /v1/simian`: classify a DNA matrix as simian or human
- `GET /v1/stats`: return aggregate counts and ratio
- PostgreSQL persistence
- Flyway migrations via Docker Compose
- Swagger UI docs served from the app

## Tech Stack

- C++17
- [Crow](https://crowcpp.org/)
- PostgreSQL 15
- Flyway
- CMake + vcpkg
- Docker / Docker Compose

## Project Structure

```text
.
|- src/
|  |- controllers/
|  |- routes/
|  |- services/
|  |- utils/
|  |- app.cpp
|  |- main.cpp
|- db/migrations/
|- public/swagger/
|- CMakeLists.txt
|- CMakePresets.json
|- Dockerfile
|- docker-compose.yml
|- vcpkg.json
```

## API Endpoints

- `POST /v1/simian`
- `GET /v1/stats`
- `GET /docs`
- `GET /openapi.yaml`

## Run With Docker Compose

From project root:

```bash
docker compose up --build
```

Services started:

- API: `http://localhost:8080`
- PostgreSQL: `localhost:5432`

Flyway runs migrations automatically before API starts.

## Run Locally

### 1. Install system dependencies (Ubuntu/Debian)

```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake git curl zip unzip tar pkg-config libpq-dev
```

### 2. Ensure `VCPKG_ROOT` is set

```bash
export VCPKG_ROOT=/path/to/vcpkg
```

### 3. Configure and build

```bash
cmake --preset=default
cmake --build build
```

### 4. Start PostgreSQL and apply migrations

You can use Docker Compose for DB and Flyway only:

```bash
docker compose up db flyway
```

### 5. Run API

```bash
./build/simian
```

## Environment Variables

Used by the API database layer:

- `DB_HOST` (default: `localhost`)
- `DB_PORT` (default: `5432`)
- `DB_NAME` (default: `simian`)
- `DB_USER` (default: `postgres`)
- `DB_PASSWORD` (default: `postgres`)

`docker-compose.yml` already provides these values for the `api` service.

## Example Requests

### Check DNA

```bash
curl -i -X POST http://localhost:8080/v1/simian \
  -H "Content-Type: application/json" \
  -d '{"dna":["ATGCGA","CAGTGC","TTATGT","AGAAGG","CCCCTA","TCACTG"]}'
```

Expected:

- `200 OK` for simian DNA
- `403 Forbidden` for human DNA
- `400 Bad Request` for invalid payload

### Stats

```bash
curl -s http://localhost:8080/v1/stats
```

Example response:

```json
{
  "count_mutant_dna": 40,
  "count_human_dna": 100,
  "ratio": 0.4
}
```

## API Docs

- Swagger UI: `http://localhost:8080/docs`

## GCP Deployment Pipeline

This repository now includes a deployment pipeline stack:

- Ansible: `ansible/`
- GitHub Actions:
  - `.github/workflows/ci.yml`
  - `.github/workflows/deploy-vm.yml`

### What the deploy workflow does

1. Authenticates to Docker Hub.
2. Builds and pushes an immutable API image to Docker Hub tagged with commit SHA.
3. Connects to your manually created VM over SSH.
4. Uses Ansible to pull that exact image and run Docker Compose.

### Required GitHub secrets

- `DOCKERHUB_USERNAME`
- `DOCKERHUB_TOKEN`
- `POSTGRES_DB`
- `POSTGRES_USER`
- `POSTGRES_PASSWORD`
- `DB_PORT` (optional; defaults to `5432`)
- `VM_IP`
- `VM_SSH_PRIVATE_KEY`
- `VM_SSH_USER` (optional; defaults to `ubuntu`)

### Required GitHub repository variables

- `DOCKERHUB_REPOSITORY` (for example: `simian`)

### One-time setup notes

1. Create a Docker Hub access token and store it in `DOCKERHUB_TOKEN`.
2. Ensure the VM has outbound internet access to pull Docker Hub images.
3. Run `deploy-gce` manually once via GitHub Actions `workflow_dispatch`.
