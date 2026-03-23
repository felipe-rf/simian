CREATE TABLE dna (
    id SERIAL PRIMARY KEY,
    hash TEXT UNIQUE NOT NULL,
    sequence TEXT NOT NULL,
    is_simian BOOLEAN NOT NULL
);

CREATE UNIQUE INDEX idx_dna_hash ON dna(hash);