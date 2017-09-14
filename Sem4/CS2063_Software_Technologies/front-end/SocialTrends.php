<!DOCTYPE HTML>
<html>

<head>  
<style type="text/css">
  .boxed {
  background: #2db34a;
  top: 0;
  left: 0;
  padding: 10px 0;
  right: 0;
  
}
a {
  padding: 20px;
  color: #ffffff;
  text-decoration: none !important;
}
#inputform {


}
#logo {
  display: inline-block;
}
#city {
  display: inline-block;
  position: fixed;
  top:100px;
  left:500px;
 
}
#type{
position: fixed;
  top:100px;
  left:650px;
}

#submitbutton{
position: fixed;
  top:100px;
  left:800px;
}
#inputelements {
  padding-left: 500px;
  padding-top: 0px;
}

</style>
<?php
$array = array();
$dbhost = '127.0.0.1:3306';
$dbuser = 'root';
$dbpass = '';
$conn = mysql_connect($dbhost, $dbuser, $dbpass);
$url = $_SERVER['REQUEST_URI'];
$parts = parse_url($url);
parse_str($parts['query'], $query);
$param1 = $query['city'];
$param2 = $query['type'];

if(! $conn )
{
  die('Could not connect: ' . mysql_error());
}
$sql = 'SELECT PostingTimestamp,shareCount,message,UserCommentsCount,LikeCount FROM TrafficProcessedData where OrganizationName like "%hello%"';
if($param1 != ""){
$sql = str_replace("hello", $param1, $sql);
}
else{
 $sql = str_replace("hello", "hyderabad", $sql); 
}
if($param2 == ""){
  $param2 = "line";
}

mysql_select_db('test');
$retval = mysql_query( $sql, $conn );
if(! $retval )
{
  //die('Could not get data: ' . mysql_error());
}
//echo $retval;
while($row = mysql_fetch_array($retval, MYSQL_ASSOC))
{
   
    array_push($array, $row);

} 
$json = json_encode($array,JSON_NUMERIC_CHECK );
//echo $json;
mysql_free_result($retval);
mysql_close($conn);
?>
  <script type="text/javascript">
  window.onload = function () {
    var datafromphp = <?php echo $json; ?>;
    var result = []; 
    var result1 = [];
    var result2 = [];

    for(var i=0;i<datafromphp.length;i++){
      var j = 0;
      for(j=0;j<result.length;j++){
        if(result[j].x.getDate() == (new Date(datafromphp[i].PostingTimestamp)).getDate() && result[j].x.getFullYear() == (new Date(datafromphp[i].PostingTimestamp)).getFullYear() && result[j].x.getMonth() == (new Date(datafromphp[i].PostingTimestamp)).getMonth()){
   
          if(result[j].y < datafromphp[i].shareCount){
            result[j].y = datafromphp[i].shareCount;
            result[j].message = datafromphp[i].message;

          }
            break;
          
        }

      }
      if(j == result.length){
        result.push({
        x: new Date(datafromphp[i].PostingTimestamp),
        y: datafromphp[i].shareCount,
        message: datafromphp[i].message 
      });
      }
      if(result.length == 30){
        break;
      }
    }


var dat1 = [];
dat1.push(
  {        
        type: <?php echo "\"$param2\""; ?>, 
        showInLegend: true,
        name: "Share Count",
        //markerSize: 0,        
        dataPoints: result
  }
  );
for(var i =0 ; i < result.length ; i++){
  dat1.push(
  {        
        type: <?php echo "\"$param2\""; ?>, 
        showInLegend: false,
        name: result[i].message,
        //markerSize: 0,        
        dataPoints: [{
          x: result[i].x,
          y: result[i].y
        }]
  }
  );
}
    for(var i=0;i<datafromphp.length;i++){
      var j = 0;
      for(j=0;j<result1.length;j++){
        if(result1[j].x.getDate() == (new Date(datafromphp[i].PostingTimestamp)).getDate() && result1[j].x.getFullYear() == (new Date(datafromphp[i].PostingTimestamp)).getFullYear() && result1[j].x.getMonth() == (new Date(datafromphp[i].PostingTimestamp)).getMonth()){
     
          if(result1[j].y < datafromphp[i].UserCommentsCount){
            result1[j].y = datafromphp[i].UserCommentsCount;
            result1[j].message = datafromphp[i].message;

          }
            break;
          
        }

      }
      if(j == result1.length){
        result1.push({
        x: new Date(datafromphp[i].PostingTimestamp),
        y: datafromphp[i].UserCommentsCount,
        message: datafromphp[i].message 
      });
      }
      if(result1.length == 30){
        break;
      }
    }

 var dat2 = [];
dat2.push(
  {        
        type: <?php echo "\"$param2\""; ?>, 
        showInLegend: true,
        name: "Comment Count",
        //markerSize: 0,        
        dataPoints: result1
  }
  );
for(var i =0 ; i < result1.length ; i++){
  dat2.push(
  {        
        type: <?php echo "\"$param2\""; ?>, 
        showInLegend: false,
        name: result1[i].message,
        //markerSize: 0,        
        dataPoints: [{
          x: result1[i].x,
          y: result1[i].y
        }]
  }
  );
}   

for(var i=0;i<datafromphp.length;i++){
      var j = 0;
      for(j=0;j<result2.length;j++){
        if(result2[j].x.getDate() == (new Date(datafromphp[i].PostingTimestamp)).getDate() && result2[j].x.getFullYear() == (new Date(datafromphp[i].PostingTimestamp)).getFullYear() && result2[j].x.getMonth() == (new Date(datafromphp[i].PostingTimestamp)).getMonth()){
   
          if(result2[j].y < datafromphp[i].LikeCount){
            result2[j].y = datafromphp[i].LikeCount;
            result2[j].message = datafromphp[i].message;

          }
            break;
          
        }

      }
      if(j == result2.length){
        result2.push({
        x: new Date(datafromphp[i].PostingTimestamp),
        y: datafromphp[i].LikeCount,
        message: datafromphp[i].message 
      });
      }
      if(result2.length == 30){
        break;
      }
    }


var dat3 = [];
dat3.push(
  {        
        type: <?php echo "\"$param2\""; ?>, 
        showInLegend: true,
        name: "Like Count",
        //markerSize: 0,        
        dataPoints: result2
  }
  );
for(var i =0 ; i < result2.length ; i++){
  dat3.push(
  {        
        type: <?php echo "\"$param2\""; ?>, 
        showInLegend: false,
        name: result2[i].message,
        //markerSize: 0,        
        dataPoints: [{
          x: result2[i].x,
          y: result2[i].y
        }]
  }
  );
}
    var chart = new CanvasJS.Chart("chartContainer",
    {      
      title:{
        text: "Most shared posts"
      },
      animationEnabled: true,
      axisY :{
        includeZero: false
      },
      toolTip: {
        shared: "true"
      },
      data: dat1
    });

chart.render();
var chart = new CanvasJS.Chart("chartContainer1",
    {      
      title:{
        text: "Most commented posts"
      },
      animationEnabled: true,
      axisY :{
        includeZero: false
      },
      toolTip: {
        shared: "true"
      },
      data: dat2
    });
chart.render();
var chart = new CanvasJS.Chart("chartContainer2",
    {      
      title:{
        text: "Most Liked posts"
      },
      animationEnabled: true,
      axisY :{
        includeZero: false
      },
      toolTip: {
        shared: "true"
      },
      data: dat3
    });
chart.render();
}
</script>
<script type="text/javascript" src="canvasjs.min.js"></script>
</head>
<body>

<div class="boxed">
<a href="">Social Search</a>
<a href="">Social Trends</a>
<a href="">Social Analysis</a>
</div>
 
  <form>
  <div id="logo">
  <img src="logo.png" alt="Logo" width=250px />
  </div>
  <div id="inputform">
  
  <div id="inputelements">
  <div id="city">
    <select name="city">
  <option <?php if($param1 == 'Hyderabad'){echo("selected");}?>>Hyderabad</option>
  <option value="Bengaluru" <?php if($param1 == 'Bengaluru'){echo("selected");}?>>Bangalore</option>
  <option <?php if($param1 == 'Delhi'){echo("selected");}?>>Delhi</option>
  <option <?php if($param1 == 'Kolkata'){echo("selected");}?>>Kolkata</option>
    </select>
    </div>
    <div id="type">
    <select name="type">
      <option value="line" <?php if($param2 == 'line'){echo("selected");}?>>line</option>
      <option value="spline" <?php if($param2 == 'spline'){echo("selected");}?>>Curved Line</option>
      <option value="area" <?php if($param2 == 'area'){echo("selected");}?>>Area</option>
      <option value="splineArea" <?php if($param2 == 'splineArea'){echo("selected");}?>>Curved Area</option>
    </select>
    </div>
    <div id="submitbutton">
    <button type="submit">Get Graph</button>
    </div>
    </div>
    </div>
  </form>
  
  <div id="chartContainer" style="height: 300px; width: 100%;">
  </div>
  <div id="chartContainer1" style="height: 300px; width: 100%;">
  </div>
  <div id="chartContainer2" style="height: 300px; width: 100%;">
  </div>

</body>
</html>
