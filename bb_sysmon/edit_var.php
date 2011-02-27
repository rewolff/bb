<?

require_once 'db.php';
require_once 'bb.php';

$vn = $_GET["varname"];

$curval = get_bb_val ($vn);

echo "<form method=\"get\" action=\"change_value.php\"> \n";
echo "<input type=hidden name=varname value=\"$vn\">\n";
echo "$vn = $curval <input type=\"text\" name=newvalue size=15 value=\"$curval\">";
echo "<input type=submit value=change>\n";

