<?

putenv ("BB_BASE=/var/bb/bb");


// Will be /usr or /usr/local later on, but for now 
// work with my development copy. 
$BB_BINDIR="/home/wolff/bb/";


function get_bb_val ($name)
{
  global $BB_BINDIR;

  $fp = popen ("$BB_BINDIR/bb_print $name", "r");
  $val = rtrim (fgets ($fp));
  pclose ($fp);
  return $val;
}

?>