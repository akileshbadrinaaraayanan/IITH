from django.db import models


# Create your models here.
class Login(models.Model):
    #id=models.IntegerField(primary_key=True)
    username = models.CharField(max_length=20)
    password = models.CharField(max_length=20)

    def __str__(self):
        return str(self.id)


class Detail(models.Model):
    #id=models.ForeignKey(Login,on_delete=models.CASCADE,primary_key=True)
    id=models.ForeignKey(Login,on_delete=models.CASCADE,primary_key=True,auto_created=True)
    firstname = models.CharField(max_length=50)
    lastname = models.CharField(max_length=50)
    email = models.CharField(max_length=50)
    mobile = models.IntegerField()

    def __str__(self):
        return str(self.id)

