#! /Users/user/Documents/CMC/prac4/server/server/mjs
<SCRIPT LANGUAGE="mjs" RUNAT="server">
{
	/*types conversation*/
	var x = 3;
	var y = true;
	var z = "asd";
	write(x+y, "\n");
	write(x+z, "\n");
	write(z+z, "\n");
	write(z+x,"\n");
	write(z+y,"\n");
	write(x && z, "\n");
	write(z*x);
	write(y+z, "\n");
}
</SCRIPT>