class Video:
    def __init__(self):
        self.title = ""
        self.id = ""
        self.description = ""
        self.thumbnail_url = ""
        self.thumbnail_width = 0
        self.thumbnail_height = 0
        self.channelTitle = ""
        self.duration = ""
        self.caption = ""
        self.viewCount = 0
        self.likeCount = 0


class Channel:
    def __init__(self):
        self.title = ""
        self.id = ""
        self.description = ""
        self.thumbnail_url = ""
        self.thumbnail_width = 100
        self.thumbnail_height = 100

class Playlist:
	def __init__(self):
		self.id=""
		self.title=""
		self.channelId=""
		self.channelTitle=""
		self.thumbnail_url=""
		self.thumbnail_width = 100
		self.thumbnail_height = 100 

class PlayListItem:
	def __init__(self):
		self.playlistId=""
		self.id=""
		self.title=""
		self.description=""
		self.thumbnail_url=""
		self.thumbnail_width=100
		self.thumbnail_height=100
		self.channelTitle=""

