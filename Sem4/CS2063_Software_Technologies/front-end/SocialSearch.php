

<!DOCTYPE html>
<html>
<head>

<script type="text/javascript">

</script>
<style>

.input {
	 position: absolute;
   left: 450px;
   top: 125px;
}
.logo {
	display: inline-block;
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
	background-color: #ffffff;
}
p {
	
    text-overflow: ellipsis;
    box-shadow: 0px 0px 2px #888888;
	background-color: #ffffff;
    width: 450px;
    padding: 25px;
    border: 0px;
    font-family: "Times New Roman", Times, serif;
}

.data {
	padding-top: 50px;
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
  <option value="accident">Accident</option>
  <option>Water Logging</option>
  <option>Water Logging</option>
  <option>Water Logging</option>
  <option>Water Logging</option>
  
</select>

<button type="submit" style="width: 150px;height: 20px">Browse</button>
</form>
</div>
</div>

<div class="data">
<?php
$dbhost = '127.0.0.1:3306';
$dbuser = 'root';
$dbpass = '';
$conn = mysql_connect($dbhost, $dbuser, $dbpass);

if(! $conn )
{
  die('Could not connect: ' . mysql_error());
}
$sql = 'SELECT message,id FROM TrafficProcessedData where message like "%hello%" and OrganizationName like "%abcdefg%"';
$sql = str_replace("hello", $param1, $sql);
$sql = str_replace("abcdefg", $param2, $sql);

mysql_select_db('test');
$retval = mysql_query( $sql, $conn );
if(! $retval )
{
  //die('Could not get data: ' . mysql_error());
}
//echo $retval;

while($row = mysql_fetch_array($retval, MYSQL_ASSOC))
{
	$summary = substr($row['message'], 0,200);
	echo "<p>{$summary} <a href=\"http://facebook.com/{$row['id']}\" id=\"linktofb\">...see more</a></p>";

} 
mysql_free_result($retval);
mysql_close($conn);
?>
</div>
</body>
</html>