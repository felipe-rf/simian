FROM ubuntu:22.04

# Install dependencies
RUN apt-get update && apt-get install -y \
    git cmake g++ curl zip unzip tar pkg-config libpq-dev

# Install vcpkg
WORKDIR /opt
RUN git clone https://github.com/microsoft/vcpkg.git
RUN /opt/vcpkg/bootstrap-vcpkg.sh

ENV VCPKG_ROOT=/opt/vcpkg

# Copy project
WORKDIR /app
COPY . .

# Build
RUN cmake --preset=default
RUN cmake --build build

EXPOSE 8080

CMD ["./build/simian"]