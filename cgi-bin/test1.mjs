#! /Users/user/Documents/CMC/prac4/server/server/mjs
<html><body>
text before script</br>
<SCRIPT LANGUAGE="mjs" RUNAT="server">
{
	var x = 2;
	var y = true;
	var z = false;
	var qwe = "str";
	var asd = "as\\\"a";
	if (false)
	{
		var qwedaaas = 123;
		x = x;
	}
	else
		x = 3;
	if (true)
		write(" This is true\n");
	else
		write(" This is false\n");
	write(x, y,z, " ",qwe);
	/*write(Environment["QUERY_STRING"]);*/
	write("\n");
}
</SCRIPT>
text after script</br>
</body></html>
