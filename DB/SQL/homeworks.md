# 1. Basic queries
## Отделы и сколько разных сотрудников в каждом отделе уволено. Вывод в алфавитном порядке по названию отделa
```sql
SELECT 
    count(distinct job.EMPLOYEE_ID) AS workers_count, 
    dep.DEPARTMENT_NAME
FROM JOB_HISTORY job
JOIN DEPARTMENTS dep ON job.DEPARTMENT_ID=dep.DEPARTMENT_ID
GROUP BY dep.DEPARTMENT_NAME
HAVING count(distinct job.EMPLOYEE_ID) >= 1
ORDER BY dep.DEPARTMENT_NAME
```
![alt text](resources/depsReduction.png)

## Вывод страны и города самого высокооплачиваемого менедждера

```sql
SELECT *
FROM
(
    SELECT 
        dep.manager_id, 
        emp.salary
    FROM departments dep
    LEFT JOIN employees emp ON dep.manager_id=emp.employee_id
    WHERE 
        emp.salary=
        (
            SELECT max(salary)
            FROM employees
            WHERE employee_id IN 
            (
                SELECT manager_id
                FROM departments
            )
        )
)
JOIN 
(
    SELECT 
        dep.manager_id, 
        loc.country_id, 
        loc.city
    FROM departments dep
    LEFT JOIN locations loc ON loc.location_id=dep.location_id
    WHERE dep.manager_id IS NOT NULL
)
USING(manager_id)
```
![alt text](resources/richManagerLocation.png)

# 2. Window Functions
## Определить год, в котором было трудоустроено больше всего человек

```sql
SELECT
startYear
FROM
(
    SELECT
        COUNT(employee_id) AS newWorkers,
        EXTRACT(YEAR FROM start_date) AS startYear,
        DENSE_RANK() over (order by COUNT(distinct employee_id) desc) AS dense_rank
    FROM job_history
    GROUP BY EXTRACT(YEAR FROM start_date)
)
WHERE dense_rank=1
ORDER BY dense_rank
```


