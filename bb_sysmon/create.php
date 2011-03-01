<?

require_once 'bb.php';

$vn = $_GET["varname"];
$vt = $_GET["vartype"];


$cmd = sprintf ("$BB_BINDIR/bb_create %s %s", $vn, $vt);
// echo "executing $cmd";

system ($cmd); 
header ("Location: index.php");

?>

