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
#issue {
position: fixed;
  top:130px;
  left:500px;
}

#submitbutton{
position: fixed;
  top:130px;
  left:700px;
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
$param1 = $query['issue'];

if(! $conn )
{
  die('Could not connect: ' . mysql_error());
}
$sql = 'SELECT OrganizationName FROM TrafficProcessedData where message like "%hello%"';
$sql = str_replace("hello", $param1, $sql);



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

    var hyd = 0;
    var bang = 0;
    var del = 0;
    var kol = 0;
    for(var i=0;i<datafromphp.length;i++){
      if((datafromphp[i].OrganizationName).indexOf("Hyderabad") > -1){
        hyd = hyd + 1;
      }else if((datafromphp[i].OrganizationName).indexOf("Bengaluru") > -1){
        bang = bang + 1;
      }else if((datafromphp[i].OrganizationName).indexOf("Delhi") > -1){
        del = del + 1;
      }else if((datafromphp[i].OrganizationName).indexOf("Kolkata") > -1){
        kol = kol + 1;
      }
    }
    var total = hyd + kol + del + bang;
      result.push({
        y : hyd*100/total,
        legendText: "Hyderabad",
        label: "Hyderabad"
      })

      result.push({
        y : bang*100/total,
        legendText: "Bangalore",
        label: "Bangalore"
      })

      result.push({
        y : del*100/total,
        legendText: "Delhi",
        label: "Delhi"
      })

      result.push({
        y : kol*100/total,
        legendText: "Kolkata",
        label: "Kolkata"
      })
    
    
//document.getElementById("demo").innerHTML = result[2].y


var chart = new CanvasJS.Chart("chartContainer",
  {
    title:{
      text: "Issue comparision between cities"
    },
                animationEnabled: true,
    legend:{
      verticalAlign: "center",
      horizontalAlign: "left",
      fontSize: 20,
      fontFamily: "Helvetica"        
    },
    theme: "theme2",
    data: [
    {        
      type: "pie",       
      indexLabelFontFamily: "Garamond",       
      indexLabelFontSize: 20,
      indexLabel: "{label} {y}%",
      startAngle:-20,      
      showInLegend: true,
      toolTipContent:"{legendText} {y}%",
      dataPoints: result
    }
    ]
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
  <div id="issue">
  <select name="issue" style="width: 150px">
  <option value="water logging" <?php if($param1 == 'water logging'){echo("selected");}?>>Water Logging</option>
  <option value="helmet" <?php if($param1 == 'helmet'){echo("selected");}?>>Helmet</option>
  <option value="accident"<?php if($param1 == 'accident'){echo("selected");}?>>Accident</option>
  <option value="traffic" <?php if($param1 == 'traffic'){echo("selected");}?>>Traffic</option>
  <option value="complaint" <?php if($param1 == 'complaint'){echo("selected");}?>>Complaint</option>
  <option>Water Logging</option>
  <option>Water Logging</option>
  </select>
  </div>
    <div id="submitbutton">
    <button type="submit">Get Graph</button>
    </div>
    </div>
    </div>
  </form>
  
  <div id="chartContainer" style="height: 600px; width: 900px;padding-left:150px;">
  </div>


</body>
</html>
