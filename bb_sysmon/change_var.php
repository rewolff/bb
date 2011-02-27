<?

require_once 'db.php';
require_once 'bb.php';

$vn = $_GET["varname"];
$val = $_GET["newvalue"];

system ("/home/wolff/bb/bb_set $varname $newvalue");

header ("Location: index.php");

?>