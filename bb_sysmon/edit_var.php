<?

require_once 'db.php';
require_once 'bb.php';

$vn = $_GET["varname"];

$curval = get_bb_val ($vn);

echo "<h1>manually change value</h1>\n";
echo "<form method=\"get\" action=\"change_value.php\"> \n";
echo "<input type=hidden name=varname value=\"$vn\">\n";
echo "$vn : <input type=\"text\" name=newvalue size=15 value=\"$curval\">";
echo "<input type=submit value=change>\n";
#
#
echo "<h1>automatically generate value</h1>\n";


echo "<form method=\"get\" action=\"change_auto.php\"> \n";
echo "<input type=hidden name=varname value=\"$vn\">\n";
echo "$vn : <input type=\"text\" name=command size=15 value=\"$curval\">";
echo "<input type=submit value=change>\n";
