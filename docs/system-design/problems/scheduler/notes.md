# Design a Job Scheduler

Job scheduler could be of many types, such as to schedule CPU jobs
(multiprocessing), web server jobs (data application), Hadoop job scheduler,
etc.

## Celery like job scheduler

Celery pick up tasks from the web server (web application) manage the tasks using
a broker service like message queue (Redis) to schedule the individual task in
distributed workers. The tasks usually run in a first come and first serve manner.

* [Celery Tutorial](https://medium.com/swlh/python-developers-celery-is-a-must-learn-technology-heres-how-to-get-started-578f5d63fab3)
* [Redis in Action - 6.4 Task queues](https://redislabs.com/ebook/part-2-core-concepts/chapter-6-application-components-in-redis/6-4-task-queues/)
* [PagerDuty Scheduler Design](https://github.com/PagerDuty/scheduler/blob/master/doc/design.md)

## Airflow like data job scheduler

* [Azkaban](https://azkaban.github.io/)
* [Airflow](https://airflow.apache.org/docs/stable/)
