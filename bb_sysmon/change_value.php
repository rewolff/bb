<?

require_once 'db.php';
require_once 'bb.php';

$vn = $_GET["varname"];
$val = $_GET["newvalue"];

system ("/home/wolff/bb/bb_set $vn $val");

header ("Location: index.php");

?>