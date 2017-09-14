

<!DOCTYPE html>
<html>
<head>


<style>

.input {
	 position: absolute;
   left: 30px;
   top: 300px;
}

.logo {
	display: inline-block;
  padding-bottom: 40px;
}
.header {

}
#inbox{
  padding-left: 10px;
  padding-top: 10px;
}

#boxed {
	  background-color: #2db34a;
    height: 40px;
}
body {
	background-color: #fafafa;
}
p {
	  line-height: 150%;
    text-overflow: ellipsis;
    font-size: 19px;
	/*background-color: #ffffff;*/
    box-shadow: 0px 0px 2px #888888;
    width: 700px;
    padding-left: 40px;
    padding-bottom: 8px;
    color: #414141;
    /*padding-right: 70px;
    padding-bottom: 10px; */

    border: 0px;
    font-family: "Times New Roman", Times, serif;
}

#data {
  position:absolute;
  left:350px;
  top:150px;
  width:800px;
  padding-top: 40px;
  padding-left: 60px;
  background-color: #fdfdfd;
  box-shadow: 0px 0px 2px #888888;
}
/*
#update{
  position: absolute;
  left:700px;
  top:200px;
}*/
button.link{
  padding-left: 200px;
  background: none;
  border: none;
  text-decoration: underline;
  color:#3366ff;

}
#submit_div{
  padding-left: 50px;
}

a:link {
    text-decoration: none;
}

a:visited {
    text-decoration: none;
}

a:hover {
    text-decoration: underline;
}

a:active {
    text-decoration: underline;
}

#linktofb {
  color: #3366CC;
}
#gotoanalysis{
  color: #ffffff;
  padding-left: 0px;
  text-decoration: none;
}
#gotocategory {
  color: #ffffff;
  padding-left: 20px;
  text-decoration: none;

}
</style>
</head>
<body>




<div id="boxed">
<div id="inbox">
<a href="analysis.php" id="gotoanalysis">Graphical analysis</a>
<a href="" id="gotocategory">Browse by category</a>
</div>
</div>

<div class="header">
<div class="logo">
<img src="logo.png" width=250px />
</div>
<div class="input">
<form>
<?php 
 $url = $_SERVER['REQUEST_URI'];
$parts = parse_url($url);
parse_str($parts['query'], $query);
$param1 = $query['issue'];
$param2 = $query['city'];
$param3 = $query['search_word'];
 ?>
<select id="city" name="city" style="width: 150px">
  <option <?php if($param2 == 'Hyderabad'){echo("selected");}?>>Hyderabad</option>
  <option value="Bengaluru" <?php if($param2 == 'Bengaluru'){echo("selected");}?>>Bangalore</option>
  <option <?php if($param2 == 'Delhi'){echo("selected");}?>>Delhi</option>
  <option <?php if($param2 == 'Kolkata'){echo("selected");}?>>Kolkata</option>
</select>

<select id="issue" name="issue" style="width: 150px">
  <option value="water logging" <?php if($param1 == 'water logging'){echo("selected");}?>>Water Logging</option>
  <option value="helmet" <?php if($param1 == 'helmet'){echo("selected");}?>>Helmet</option>
  <option value="accident"<?php if($param1 == 'accident'){echo("selected");}?>>Accident</option>
  <option value="traffic" <?php if($param1 == 'traffic'){echo("selected");}?>>Traffic</option>
  <option value="complaint" <?php if($param1 == 'complaint'){echo("selected");}?>>Complaint</option>
  <option>Water Logging</option>
  <option>Water Logging</option>
  
</select>
<br>

<div id="advancedsearch"></div>
<div id="submit_div"><button type="submit" style="width: 150px;height: 20px;">Browse</button></div>
<script type="text/javascript">
  var x=0;
    function filldiv(){
      
      if(x>0){
            x=0;
            document.getElementById("advancedsearch").innerHTML=""; 
      }
      else{
        x=1;
       document.getElementById("advancedsearch").innerHTML="<input name=\"search_word\" id=\"search_input\" placeholder=\"search\">";
      }
    }
</script>
</form>
<button class="link" onclick="filldiv()">advanced search</button>
</div>
</div>

<div id="data" align="left" >
<?php
$dbhost = '127.0.0.1:3306';
$dbuser = 'root';
$dbpass = '';
$conn = mysql_connect($dbhost, $dbuser, $dbpass);

if(! $conn )
{
  die('Could not connect: ' . mysql_error());
}
$sql = 'SELECT message,id,PostingTimestamp FROM TrafficProcessedData where message like "%hello%" and OrganizationName like "%abcdefg%"';
$sql = str_replace("abcdefg", $param2, $sql);
if($param3){
$sql = str_replace("hello", $param3, $sql);
}else{
  $sql = str_replace("hello", $param1, $sql);
}

mysql_select_db('test');
$retval = mysql_query( $sql, $conn );
if(! $retval )
{
  //die('Could not get data: ' . mysql_error());
}
//echo $retval;
$numOfPosts = 0;
while($row = mysql_fetch_array($retval, MYSQL_ASSOC))
{
  $numOfPosts = 1;
	$summary = substr($row['message'], 0,200);
	echo "<p>{$summary} <a href=\"http://facebook.com/{$row['id']}\" id=\"linktofb\" target=\"_blank\">...see more</a><br>{$row['PostingTimestamp']}</p>";

}

if($numOfPosts == 0){
  echo "<h2 align=\"center\" position=\"absolute\" top=\"40px\">No posts to display</h2>";
}
mysql_free_result($retval);
mysql_close($conn);
?>

</div>
<!--
<div id="update">

<?php
$dbhost = '127.0.0.1:3306';
$dbuser = 'root';
$dbpass = '';
$conn = mysql_connect($dbhost, $dbuser, $dbpass);

if(! $conn )
{
  die('Could not connect: ' . mysql_error());
}
mysql_select_db('test');
$today = date("Y-m-d");
$sql = 'SELECT message FROM TrafficProcessedData where message like "%hello%" and OrganizationName like "%abcdefg%" and PostingTimestamp = "lalala"';
$sql = str_replace("hello", "Traffic At", $sql);
$sql = str_replace("abcdefg", "bengaluru", $sql);
$sql = str_replace("lalala", $today, $sql);


$retval = mysql_query( $sql, $conn );
if(! $retval )
{
  //die('Could not get data: ' . mysql_error());
}
//echo $retval;

while($row = mysql_fetch_array($retval, MYSQL_ASSOC))
{
  //$summary = substr($row['message'], 0,200);
  echo "<p>{$row['message']} </p>";

} 

$sql = 'SELECT message FROM TrafficProcessedData where message like "%hello%" and OrganizationName like "%abcdefg%" and PostingTimestamp = "lalala"';
$sql = str_replace("hello", "Traffic Live Update", $sql);
$sql = str_replace("abcdefg", "hyderabad", $sql);
$sql = str_replace("lalala", $today, $sql);


$retval = mysql_query( $sql, $conn );
if(! $retval )
{
  //die('Could not get data: ' . mysql_error());
}
//echo $retval;

while($row = mysql_fetch_array($retval, MYSQL_ASSOC))
{
  //$summary = substr($row['message'], 0,200);
  echo "<p>{$row['message']} </p>";

} 

$sql = 'SELECT message FROM TrafficProcessedData where message like "%hello%" and OrganizationName like "%abcdefg%" and PostingTimestamp = "lalala"';
$sql = str_replace("hello", "Traffic Advisory", $sql);
$sql = str_replace("abcdefg", "Delhi", $sql);
$sql = str_replace("lalala", $today, $sql);


$retval = mysql_query( $sql, $conn );
if(! $retval )
{
  //die('Could not get data: ' . mysql_error());
}
//echo $retval;

while($row = mysql_fetch_array($retval, MYSQL_ASSOC))
{
  //$summary = substr($row['message'], 0,200);
  echo "<p>{$row['message']} </p>";

} 

$sql = 'SELECT message FROM TrafficProcessedData where message like "hello%" and OrganizationName like "%abcdefg%" and PostingTimestamp = "2015-04-30"';
$sql = str_replace("hello", "on", $sql);
$sql = str_replace("abcdefg", "kolkata", $sql);
$sql = str_replace("lalala", $today, $sql);


$retval = mysql_query( $sql, $conn );
if(! $retval )
{
  //die('Could not get data: ' . mysql_error());
}
//echo $retval;

while($row = mysql_fetch_array($retval, MYSQL_ASSOC))
{
  //$summary = substr($row['message'], 0,200);
  echo "<p>{$row['message']} </p>";

} 
mysql_free_result($retval);
mysql_close($conn);
?>
</div>
-->
</body>
</html>