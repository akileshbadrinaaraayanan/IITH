from django.shortcuts import render
from django.http import HttpResponse, HttpResponseRedirect
from .models import *
import apiclient
from apiclient.discovery import build
from django.core.mail import send_mail,EmailMessage
from apiclient.errors import HttpError
from oauth2client.tools import argparser
from django.core.cache import cache
from oauth2client.client import flow_from_clientsecrets
from oauth2client.file import Storage
from oauth2client.tools import argparser, run_flow
import os
import httplib2
import sys
#import urllib3
import json
# from music.tasks import send_feedback_email_task


from celery.decorators import task
from celery.utils.log import get_task_logger
from dbms import celery_app


logger = get_task_logger(__name__)

#import classes
from music.classes import *

DEVELOPER_KEY = "AIzaSyC4lxc1NfUV09y_vX9kTiRKvSbK6bc6rP0"
YOUTUBE_API_SERVICE_NAME = "youtube"
YOUTUBE_API_VERSION = "v3"
# # This OAuth 2.0 access scope allows for full read/write access to the
# # authenticated user's account.
# YOUTUBE_READ_WRITE_SCOPE = "https://www.googleapis.com/auth/youtube"

# CLIENT_SECRETS_FILE = "client_secrets.json"


# # This variable defines a message to display if the CLIENT_SECRETS_FILE is
# # missing.
# MISSING_CLIENT_SECRETS_MESSAGE = """
# WARNING: Please configure OAuth 2.0

# To make this sample run you will need to populate the client_secrets.json file
# found at:

#    %s

# with information from the {{ Cloud Console }}
# {{ https://cloud.google.com/console }}

# For more information about the client_secrets.json file format, please visit:
# https://developers.google.com/api-client-library/python/guide/aaa_client_secrets
# """ % os.path.abspath(os.path.join(os.path.dirname(__file__),
#                                    CLIENT_SECRETS_FILE))


# def get_authenticated_service(args):
#     flow = flow_from_clientsecrets(CLIENT_SECRETS_FILE,
#                                    scope=YOUTUBE_READ_WRITE_SCOPE,
#                                    message=MISSING_CLIENT_SECRETS_MESSAGE)

#     #storage = Storage("%s-oauth2.json" % sys.argv[0])
#     storage = Storage("subscriptions-oauth2.json")
#     credentials = storage.get()

#     if credentials is None or credentials.invalid:
#         credentials = run_flow(flow, storage, args)

#     return build(YOUTUBE_API_SERVICE_NAME, YOUTUBE_API_VERSION,
#                  http=credentials.authorize(httplib2.Http()))




# # argparser.add_argument("--user", help="ID of the channel to subscribe to.",
# #                            default="Phaneendra Babu")
# # args = argparser.parse_args()

# #args = argparser.parse_args()
# args=""
# youtube = get_authenticated_service(args)


youtube = build(YOUTUBE_API_SERVICE_NAME, YOUTUBE_API_VERSION,
                developerKey=DEVELOPER_KEY)


# class Video:
#     def __init__(self):
#         self.title = ""
#         self.id = ""
#         self.description = ""
#         self.thumbnail_url = ""
#         self.thumbnail_width = 0
#         self.thumbnail_height = 0
#         self.channelTitle = ""
#         self.duration = ""
#         self.caption = ""
#         self.viewCount = 0
#         self.likeCount = 0


# class Channel:
#     def __init__(self):
#         self.title = ""
#         self.id = ""
#         self.description = ""
#         self.thumbnail_url = ""
#         self.thumbnail_width = 100
#         self.thumbnail_height = 100

# class Playlist:
# 	def __init__(self):
# 		self.id=""
# 		self.title=""
# 		self.channelId=""
# 		self.channelTitle=""
# 		self.thumbnail_url=""
# 		self.thumbnail_width = 100
# 		self.thumbnail_height = 100 

# class PlayListItem:
# 	def __init__(self):
# 		self.playlistId=""
# 		self.id=""
# 		self.title=""
# 		self.description=""
# 		self.thumbnail_url=""
# 		self.thumbnail_width=100
# 		self.thumbnail_height=100
# 		self.channelTitle=""



# Create your views here.
def home(request):
    return render(request, 'music/home.html', {})


def login(request):
	m=""
	#m=send_feedback_email_task.delay("HI It is our DBMS Project").result
	print("Hello")
	print("Message:  %s" % m)
	#print(add.delay(4,5).get())
	return render(request, 'music/login.html', {})


def register(request):
    return render(request, 'music/register.html', {})


def savedetails(request):
    firstname = request.POST["firstname"]
    lastname = request.POST["lastname"]
    email = request.POST["email"]
    mobile = request.POST["mobile"]
    username = request.POST["username"]
    password = request.POST["password"]
    try:
        o = Login.objects.get(username=username)
        return render(request, 'music/register.html', {'error_message': username + " already taken"})
    except (KeyError, Login.DoesNotExist):
        l = Login(username=username, password=password)
        l.save()
        l.detail_set.create(firstname=firstname, lastname=lastname, email=email, mobile=mobile)
        return render(request, 'music/login.html', {'error_message': "Account Successfully Registered.Login Here"})



def validate(request):
    uname = request.POST["username"]
    pwd = request.POST["password"]
    try:
        user = Login.objects.get(username=uname)
    except (KeyError, Login.DoesNotExist):
        return render(request, 'music/login.html', {'error_message': "Username is not found in database"})
    else:

        if pwd == user.password:

            # return HttpResponseRedirect('music:user', args=(user.id,))
            detail = Detail.objects.get(pk=user.id)
            send_mail("Conformation of DBMS Accout","PLease Click Below link to confirm your email you registered on DBMS",
                  'cs13b1037@iith.ac.in',['cs13b1042@iith.ac.in'],fail_silently=True)
            
            #popular_videos = cache.get_or_set('popular',popular(),100000)
            popular_videos=get_popular_videos()

            # popular_videos=cache.get('popular_videos')
            # if popular_videos is None:
            # 	print("Not cached")
            # 	popular_videos=popular()
            # 	cache.set('popular_videos',popular_videos,600)

           
            #popular_channels = cache.get_or_set('popular_channels',popular_channels(),1000000)

            popular_channels_list = popular_channels()
            context = {
                'id': user.id,
                'fullname': detail.firstname + detail.lastname,
                'email': detail.email,
                 'popular_videos': popular_videos,
                'popular_channels':popular_channels_list,
            }

            return render(request, 'music/user.html', context)
        else:
            return render(request, 'music/login.html', {'error_message': "Incorrect Username,Password Combination"})


def user(request, id):
    return render(request, "music/user.html", {'id': id})


def search(request):
    query = request.POST["search"]
    search_response = youtube.search().list(
            q=query,
            part="id,snippet",
            maxResults=5
    ).execute()

    videos = []
    channels = []
    # playlists = []

    if 'nextPageToken' in search_response:
        print(search_response['nextPageToken'])
        #channels2,videos2=get_next_page.delay(search_response['nextPageToken']).get()
        #videos2=get_next_page.delay(search_response['nextPageToken']).get()
        print("got next page")

    for search_result in search_response.get("items", []):
        # print search_result
        # if "snippet" in search_result and "thumbnails" in search_result["snippet"] and "default" in search_result["snippet"]["thumbnails"]:
        #     print search_result["snippet"]["thumbnails"]["default"]
        if search_result["id"]["kind"] == "youtube#video":
            v = Video()
            if "id" in search_result and "videoId" in search_result["id"]:
                v.id = search_result["id"]["videoId"]
            get_info(v, search_result)
            videos.append(v)
        elif search_result["id"]["kind"] == "youtube#channel":
            ch = Channel()
            get_channel_info(ch, search_result)
            channels.append(ch)
    return render(request, 'music/search.html', {'query': query, 'videos': videos, 'channels': channels})


def watch(request, id):
    related_videos = related(id)
    return render(request, 'music/watch.html', {'id': id, 'related_videos': related_videos})

# channel playlists can be obtained from playlist.list or using contentDetails in channel
# channel["contentDetails"]["relatedPlaylists"]["uploads"]
def channel(request,id):
	search_response = youtube.playlists().list(
	        channelId=id,
	        part="id,snippet"
	).execute()
	playlists=[]
	print("Channel Id : ",id)
	for search_result in search_response.get("items",[]):
		pl=Playlist()
		if "id" in search_result:
			pl.id=search_result["id"]
		print("Playlist Id :  ",pl.id)
		pl.title=search_result["snippet"]["title"]
		pl.channelId=search_result["snippet"]["channelId"]
		pl.channelTitle=search_result["snippet"]["channelTitle"]
		pl.thumbnail_url=search_result["snippet"]["thumbnails"]["default"]["url"]
		pl.thumbnail_width=search_result["snippet"]["thumbnails"]["default"]["width"]
		pl.thumbnail_height=search_result["snippet"]["thumbnails"]["default"]["height"]
		playlists.append(pl)
	context={
		'channel_id':id,
		'playlists':playlists,
	}
	return render(request,'music/channel.html',context)

def playlist(request,id):
	search_response = youtube.playlistItems().list(
	        playlistId=id,
	        part="id,snippet"
	).execute()
	playlistItems=[]
	for search_result in search_response.get("items",[]):
		pli=PlayListItem()
		pli.playlistId=search_result["id"]
		pli.id=search_result["snippet"]["resourceId"]["videoId"]
		pli.title=search_result["snippet"]["title"]
		pli.description=search_result["snippet"]["description"]
		pli.thumbnail_url=search_result["snippet"]["thumbnails"]["default"]["url"]
		pli.thumbnail_width=search_result["snippet"]["thumbnails"]["default"]["width"]
		pli.thumbnail_height=search_result["snippet"]["thumbnails"]["default"]["height"]
		pli.channelTitle=search_result["snippet"]["channelTitle"]
		playlistItems.append(pli)
	context={'playlistItems':playlistItems}
	print("playlist Id : ",id)
	return render(request,'music/playlist.html',context)
	



def popular():
    # youtube = build(YOUTUBE_API_SERVICE_NAME, YOUTUBE_API_VERSION,
    #                 developerKey=DEVELOPER_KEY)
    print("Popular Request")

    video_response = youtube.videos().list(
            chart="mostPopular",
            part='id,snippet,statistics,contentDetails',
            maxResults=5,
            videoCategoryId="10",
    ).execute()

    videos = []

    # print(video_response)
    # Add each result to the list, and then display the list of matching videos.
    for video_result in video_response.get("items", []):
        v = Video()
        if "id" in video_result:
            v.id = video_result["id"]
        get_info(v, video_result)
        videos.append(v)
    # print("Videos:\n", "\n".join(videos), "\n")
    return videos

def popular_channels():
	print("popular_channels request")
	search_response = youtube.channels().list(
		categoryId="GCTXVzaWM",
		part="snippet,id,contentDetails",
		maxResults=5
	).execute()
	channels=[]
	
	for search_result in search_response.get("items",[]):
		ch=Channel()
		ch.id=search_result["id"]
		ch.description=search_result["snippet"]["description"]
		ch.title=search_result["snippet"]["title"]
		ch.thumbnail_url=search_result["snippet"]["thumbnails"]["default"]["url"]
		# ch.thumbnail_width=search_result["snippet"]["thumbnails"]["default"]["width"]
		# ch.thumbnail_height=search_result["snippet"]["thumbnails"]["default"]["height"]
		channels.append(ch)
	return channels


def related(id):
    search_response = youtube.search().list(
            type="video",
            relatedToVideoId=id,
            part="id,snippet",
            maxResults=5,
    ).execute()
    videos = []
    for search_result in search_response.get("items", []):
        if search_result["id"]["kind"] == "youtube#video":
            v = Video()
            if "id" in search_result and "videoId" in search_result["id"]:
                v.id = search_result["id"]["videoId"]
            get_info(v, search_result)
            videos.append(v)
    return videos


def get_info(v, video_result):
    # if "id" in video_result:
    #     v.id = video_result["id"]
    if "snippet" in video_result:
        if "title" in video_result["snippet"]:
            v.title = video_result["snippet"]["title"]
        if "description" in video_result["snippet"]:
            v.description = video_result["snippet"]["description"]
        if "thumbnails" in video_result["snippet"]:
            if "default" in video_result["snippet"]["thumbnails"]:
                if "url" in video_result["snippet"]["thumbnails"]["default"]:
                    v.thumbnail_url = video_result["snippet"]["thumbnails"]["default"]["url"]
                    #  print(v.thumbnail_url)
                if "width" in video_result["snippet"]["thumbnails"]["default"]:
                    v.thumbnail_width = video_result["snippet"]["thumbnails"]["default"]["width"]
                if "height" in video_result["snippet"]["thumbnails"]["default"]:
                    v.thumbnail_height = video_result["snippet"]["thumbnails"]["default"]["height"]
        if "channelTitle" in video_result["snippet"]:
            v.channelTitle = video_result["snippet"]["channelTitle"]
    if "contentDetails" in video_result:
        if "duration" in video_result["contentDetails"]:
            v.duration = video_result["contentDetails"]["duration"]
        if "caption" in video_result["contentDetails"]:
            v.caption = video_result["contentDetails"]["caption"]
    if "statistics" in video_result:
        if "viewCount" in video_result["statistics"]:
            v.viewCount = video_result["statistics"]["viewCount"]
        if "likeCount" in video_result["statistics"]:
            v.likeCount = video_result["statistics"]["likeCount"]


# channel result in search
def get_channel_info(ch, search_result):
    if "id" in search_result:
        ch.id = search_result["id"]["channelId"]
    if "snippet" in search_result:
        if "channelTitle" in search_result["snippet"]:
            ch.channelTitle = search_result["snippet"]["channelTitle"]
        if "descritption" in search_result["snippet"]:
            ch.description = search_result["snippet"]["description"]
        if "thumbnails" in search_result["snippet"]:
            if "default" in search_result["snippet"]["thumbnails"]:
                if "url" in search_result["snippet"]["thumbnails"]["default"]:
                    ch.thumbnail_url = search_result["snippet"]["thumbnails"]["default"]["url"]
                if "width" in search_result["snippet"]["thumbnails"]["default"]:
                    ch.thumbnail_width = search_result["snippet"]["thumbnails"]["default"]["width"]
                if "height" in search_result["snippet"]["thumbnails"]["default"]:
                    ch.thumbnail_height = search_result["snippet"]["thumbnails"]["default"]["height"]




@task(name="get_next_page")
def get_next_page(token):
    print("getting next page")
    logger.info("getting next page")
    search_response = youtube.search().list(
            pageToken=token,
            part="id,snippet",
            maxResults=5
    ).execute()

    videos = []
    channels = []
    # playlists = []

    for search_result in search_response.get("items", []):
        # print search_result
        # if "snippet" in search_result and "thumbnails" in search_result["snippet"] and "default" in search_result["snippet"]["thumbnails"]:
        #     print search_result["snippet"]["thumbnails"]["default"]
        if search_result["id"]["kind"] == "youtube#video":
            v = Video()
            if "id" in search_result and "videoId" in search_result["id"]:
                v.id = search_result["id"]["videoId"]
            get_info(v, search_result)
            videos.append(v)
        elif search_result["id"]["kind"] == "youtube#channel":
            ch = Channel()
            get_channel_info(ch, search_result)
            channels.append(ch)
    #return channels,videos
    tu=(channels,videos)
    return tu



@task(name="send_feedback_email_task")
def send_feedback_email_task(message):
    """sends an email when feedback form is filled successfully"""
    logger.info("Sent feedback email")
    #return send_feedback_email(email, message)
    message="HI It is our DBMS Project"
    print("Received : ",message)
    return message
    


@task(name="sum_two_numbers")
def add(x, y):
    return x + y


def get_popular_videos():
    popular_videos=cache.get('popular_videos')
    if popular_videos is None:
    	print("Not cached")
    	popular_videos=popular()
    	cache.set('popular_videos',popular_videos,120)
    return popular_videos




