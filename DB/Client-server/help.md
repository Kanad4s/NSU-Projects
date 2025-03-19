## PostgreSQL
### 1. Развертывание PostgreSQL
```bash
pg_ctl init -D mydatabase
```
### 2. Запуск
```bash
pg_ctl start -D mydatabase -l mydatabase/postgres.log
```
### 3. Подключение
```bash
psql --dbname=postgres --host=localhost --port=5432
```
### 4. Завершение
```bash
pg_ctl stop -D mydatabase -l mydatabase/postgres.log
```