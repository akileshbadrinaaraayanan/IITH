<!DOCTYPE HTML>
<html>

<head>  

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

if(! $conn )
{
  die('Could not connect: ' . mysql_error());
}
$sql = 'SELECT x,y FROM TrafficProcessedData where OrganizationName like "%hello%"';
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
    for(var i=0;i<datafromphp.length;i++){
      result.push({
        x: new Date(datafromphp[i].x),
        y: datafromphp[i].y
      });

    }
document.getElementById("demo").innerHTML = result[0];

    var chart = new CanvasJS.Chart("chartContainer",
    {

      title:{
        text: "Site Traffic",
        fontSize: 30
      },
                        animationEnabled: true,
      axisX:{

        gridColor: "Silver",
        tickColor: "silver",
        valueFormatString: "DD/YY"

      },                        
                        toolTip:{
                          shared:true
                        },
      theme: "theme2",
      axisY: {
        gridColor: "Silver",
        tickColor: "silver"
      },
      legend:{
        verticalAlign: "center",
        horizontalAlign: "right"
      },
      data: [
      {        
        type: "line",
        showInLegend: true,
        lineThickness: 2,
        name: "Share Count",
        markerType: "square",
        color: "#F08080",
        dataPoints: result
      },
      {        
        type: "line",
        showInLegend: true,
        name: "Comments Count",
        color: "#20B2AA",
        lineThickness: 2,

        dataPoints: [
        { x: "csacssd", y: 510 },
        { x: "csacssd", y: 560 },
        { x: new Date(2010,0,7), y: 540 },
        { x: new Date(2010,0,9), y: 558 },
        { x: new Date(2010,0,11), y: 544 },
        { x: new Date(2010,0,13), y: 693 },
        { x: new Date(2010,0,15), y: 657 },
        { x: new Date(2010,0,17), y: 663 },
        { x: new Date(2010,0,19), y: 639 },
        { x: new Date(2010,0,21), y: 673 },
        { x: new Date(2010,0,23), y: 660 }
        ]
      }

      
      ],
          legend:{
            cursor:"pointer",
            itemclick:function(e){
              if (typeof(e.dataSeries.visible) === "undefined" || e.dataSeries.visible) {
                e.dataSeries.visible = false;
              }
              else{
                e.dataSeries.visible = true;
              }
              chart.render();
            }
          }
    });
//document.getElementById("demo").innerHTML = chart;
chart.render();
}
</script>
<script type="text/javascript" src="canvasjs.min.js"></script>
</head>
<body>
<p id="demo"></p>
  <div id="chartContainer" style="height: 300px; width: 100%;">
  </div>
</body>
</html>
