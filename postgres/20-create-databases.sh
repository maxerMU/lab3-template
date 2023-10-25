#!/usr/bin/env bash
set -e

# TODO для создания баз прописать свой вариант
export VARIANT=3
export SCRIPT_PATH=/docker-entrypoint-initdb.d/
export PGPASSWORD=postgres
psql "user=postgres hostaddr=127.0.0.1 port=5432 password=postgres" < $SCRIPT_PATH/scripts/db-v$VARIANT.sql
