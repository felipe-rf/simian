#include "db_service.hpp"

#include <cstdlib>
#include <iostream>

#include <libpq-fe.h>

namespace {

std::string env_or_default(const char* name, const char* fallback) {
    const char* value = std::getenv(name);
    if (value == nullptr || *value == '\0') {
        return fallback;
    }
    return value;
}

PGconn* connect_db() {
    std::string conninfo =
        "host=" + env_or_default("DB_HOST", "localhost") +
        " port=" + env_or_default("DB_PORT", "5432") +
        " dbname=" + env_or_default("DB_NAME", "simian") +
        " user=" + env_or_default("DB_USER", "postgres") +
        " password=" + env_or_default("DB_PASSWORD", "postgres");

    PGconn* conn = PQconnectdb(conninfo.c_str());
    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "PostgreSQL connection failed: " << PQerrorMessage(conn) << std::endl;
        PQfinish(conn);
        return nullptr;
    }
    return conn;
}

int count_by_type(bool is_simian) {
    PGconn* conn = connect_db();
    if (conn == nullptr) {
        return 0;
    }

    const char* values[] = {is_simian ? "true" : "false"};
    PGresult* result = PQexecParams(
        conn,
        "SELECT COUNT(*) FROM dna WHERE is_simian = $1::boolean",
        1,
        nullptr,
        values,
        nullptr,
        nullptr,
        0);

    if (PQresultStatus(result) != PGRES_TUPLES_OK || PQntuples(result) != 1) {
        std::cerr << "count_by_type query failed: " << PQerrorMessage(conn) << std::endl;
        PQclear(result);
        PQfinish(conn);
        return 0;
    }

    int count = std::atoi(PQgetvalue(result, 0, 0));
    PQclear(result);
    PQfinish(conn);
    return count;
}

}

std::optional<bool> find_dna(const std::string& hash) {
    PGconn* conn = connect_db();
    if (conn == nullptr) {
        return std::nullopt;
    }

    const char* values[] = {hash.c_str()};
    PGresult* result = PQexecParams(
        conn,
        "SELECT is_simian FROM dna WHERE hash = $1 LIMIT 1",
        1,
        nullptr,
        values,
        nullptr,
        nullptr,
        0);

    if (PQresultStatus(result) != PGRES_TUPLES_OK) {
        std::cerr << "find_dna query failed: " << PQerrorMessage(conn) << std::endl;
        PQclear(result);
        PQfinish(conn);
        return std::nullopt;
    }

    if (PQntuples(result) == 0) {
        PQclear(result);
        PQfinish(conn);
        return std::nullopt;
    }

    const char* value = PQgetvalue(result, 0, 0);
    bool is_simian = value != nullptr && (value[0] == 't' || value[0] == '1');
    PQclear(result);
    PQfinish(conn);
    return is_simian;
}

void insert_dna(const std::string& hash, const std::string& sequence, bool is_simian) {
    PGconn* conn = connect_db();
    if (conn == nullptr) {
        return;
    }

    const char* values[] = {
        hash.c_str(),
        sequence.c_str(),
        is_simian ? "true" : "false"
    };

    PGresult* result = PQexecParams(
        conn,
        "INSERT INTO dna(hash, sequence, is_simian) "
        "VALUES($1, $2, $3::boolean) "
        "ON CONFLICT(hash) DO UPDATE SET "
        "sequence = EXCLUDED.sequence, "
        "is_simian = EXCLUDED.is_simian",
        3,
        nullptr,
        values,
        nullptr,
        nullptr,
        0);

    if (PQresultStatus(result) != PGRES_COMMAND_OK) {
        std::cerr << "insert_dna query failed: " << PQerrorMessage(conn) << std::endl;
    }

    PQclear(result);
    PQfinish(conn);
}

int count_simian() {
    return count_by_type(true);
}

int count_human() {
    return count_by_type(false);
}