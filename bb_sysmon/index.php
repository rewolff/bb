<?

require_once 'db.php';
require_once 'bb.php';


echo "<h1>current list. </h1>\n";
$fp = popen ("/home/wolff/bb/bb_list", "r");


echo "<table border=1>\n";

echo "<tr><th> type </th><th> name </th> <th> value </th></tr>\n";

while ($oneline = ltrim (fgets ($fp))) {
  //$oneline = preg_replace ("/^ */", "", $oneline);
  list ($type, $name) = preg_split ("/ /", $oneline);
  //echo "type = $type name = $name";
  echo "<tr><td> $type </td><td> <a href=edit_var.php?varname=$name> $name</a> </td> <td>";
  system ("/home/wolff/bb/bb_print $name");
  echo "</td></tr>\n";
 }
echo "</table>\n";
pclose ($fp); 

?>
<h1> create</h1>
<form method="get" action="create.php">
<input type="text" name="varname" size="20"/>
<select name="vartype"> 
<option>float</option>
<option>int</option>
<option>short</option>
<option>byte</option>
</select>
<input type="submit" name="create" value="create"/>
</form>


