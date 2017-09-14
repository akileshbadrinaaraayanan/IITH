from celery.decorators import task
from celery.utils.log import get_task_logger


logger = get_task_logger(__name__)


@task(name="send_feedback_email_task")
def send_feedback_email_task(message):
    """sends an email when feedback form is filled successfully"""
    logger.info("Sent feedback email")
    #return send_feedback_email(email, message)
    print("got %s" % message)
    return message


@task(name="get_next_page")
def get_next_page(options):
	print("getting next page")
	logger.info("getting next page")

# @task
# def add(x, y):
#     return x + y
