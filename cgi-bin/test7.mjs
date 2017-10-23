#! /Users/user/Documents/CMC/prac4/server/server/mjs
<SCRIPT LANGUAGE="mjs" RUNAT="server">
{
	/*for*/
	var x = 3;
	var sum = 0;
	for (;; x++)
		sum = sum + x;
	write(sum, "\n");
}
</SCRIPT>