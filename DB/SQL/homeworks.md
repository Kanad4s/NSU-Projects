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

## Выбрать среди работников Америки (region_name = 'Americas') тех, чья зарплата превосходит зарплату менеджера из Европы (region_name = 'Europe') с наибольшим количеством подчиненных.

```sql
WITH 
americanWorkers AS
(
    SELECT 
        last_name, 
        first_name, 
        country_id, 
        salary, 
        manager_id
    FROM regions
        JOIN countries USING(region_id)
        JOIN locations USING(country_id)
        JOIN departments USING(location_id)
        JOIN employees USING(department_id)
    WHERE (region_name='Americas')
),
managerss AS 
(
    SELECT 
        COUNT(employee_id), 
        manager_id,
        DENSE_RANK() OVER (ORDER BY COUNT(employee_id) DESC) AS rank
    FROM regions
        JOIN countries USING(region_id)
        JOIN locations USING(country_id)
        JOIN departments USING(location_id)
        JOIN employees USING(department_id)
    WHERE (region_name='Europe')
    GROUP BY manager_id
),
managerSalary  AS
(
    SELECT MAX(emp.salary) AS maxSalary
    FROM managerss mngs
    JOIN employees emp ON mngs.manager_id=emp.employee_id
    WHERE mngs.rank=1
)

SELECT 
    amWrks.last_name, 
    amWrks.first_name, 
    amWrks.salary
FROM americanWorkers amWrks, managerSalary
WHERE amWrks.salary >= managerSalary.maxSalary
```
![alt text](resources/workersBetterEuropeManagers.png)

## Вывести для каждого отдела из Америки (region_name = Americas) количество работников в иерархии до третьего уровня. Первый уровень - работники без руководителя, второй уровень - это их подчиненные, а третий уровень - подчиненные работников второго уровня.

```sql
WITH 
deps AS 
(
    SELECT 
        department_id, 
        manager_id
    FROM regions 
        JOIN countries USING(region_id)
        JOIN locations USING(country_id)
        JOIN departments USING(location_id)
    WHERE region_name='Americas'
    ORDER BY department_id
),
firstLevel AS 
(
    SELECT 
        department_id,
        manager_id AS employee_id
    FROM deps
    ORDER BY department_id
),
secondLevel AS 
(
SELECT
    emp.employee_id,
    fl.employee_id AS manager_id,
    fl.department_id
FROM firstLevel fl
    JOIN employees emp ON fl.employee_id=emp.manager_id AND fl.department_id=emp.department_id
ORDER BY fl.department_id
),
thirdLevel AS 
(
SELECT
    emp.employee_id,
    sl.employee_id AS manager_id,
    sl.department_id
FROM secondLevel sl
    JOIN employees emp ON sl.employee_id=emp.manager_id AND sl.department_id=emp.department_id
ORDER BY sl.department_id
),
firstLevelCount AS 
(
SELECT 
    fl.department_id,
    COUNT(fl.employee_id) AS FirstLevel
FROM firstLevel fl
GROUP BY fl.department_id
ORDER BY fl.department_id
),
secondLevelCount AS 
(
SELECT 
    department_id,
    COUNT(employee_id) AS SecondLevel
FROM secondLevel
GROUP BY department_id
ORDER BY department_id
),
thirdLevelCount AS 
(
SELECT 
    department_id,
    COUNT(employee_id) AS ThirdLevel
FROM thirdLevel
GROUP BY department_id
ORDER BY department_id
)

SELECT 
    department_id, 
    NVL(firstLevelCount.FirstLevel, 0) AS FirstLevel,
    NVL(SecondLevelCount.SecondLevel, 0) AS SecondLevel,
    NVL(ThirdLevelCount.ThirdLevel, 0) AS ThirdLevel
FROM deps
    LEFT JOIN firstLevelCount USING(department_id)
    LEFT JOIN secondLevelCount USING(department_id)
    LEFT JOIN thirdLevelCount USING(department_id)
ORDER BY department_id
```
![alt text](resources/depsWorkersByLevel.png)