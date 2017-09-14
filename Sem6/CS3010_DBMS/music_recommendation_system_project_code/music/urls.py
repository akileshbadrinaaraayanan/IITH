from django.conf.urls import url
from . import views

app_name = "music"
urlpatterns = [
    url(r'^$', views.home, name='home'),
    url(r'^login$', views.login, name='login'),
    url(r'^register/$', views.register, name='register'),
    url(r'^savedetails$', views.savedetails, name='savedetails'),
    url(r'^user$', views.validate, name='validate'),
    url(r'(?P<id>[0-9]+)/$', views.user, name='user'),
    url(r'^search$', views.search, name='search'),
    url(r'^watch/(?P<id>[a-zA-Z0-9_-]+)$', views.watch, name='watch'),
    url(r'^channel/(?P<id>[a-zA-Z0-9_-]+)$',views.channel,name='channel'),
    url(r'^playlist/(?P<id>[a-zA-Z0-9_-]+)$',views.playlist,name='playlist')
]
