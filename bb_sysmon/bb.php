<?

putenv ("BB_BASE=/var/bb/bb");


function get_bb_val ($name)
{
  $fp = popen ("/home/wolff/bb/bb_print $name", "r");
  $val = rtrim (fgets ($fp));
  pclose ($fp);
  return $val;
}

?>