<?

require_once 'bb.php';

$vn = $_GET["varname"];
$vt = $_GET["vartype"];


$cmd = sprintf ("/home/wolff/bb/bb_create %s %s", $vn, $vt);
// echo "executing $cmd";

system ($cmd); 
header ("Location: index.php");

?>

