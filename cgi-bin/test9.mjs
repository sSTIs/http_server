#! /Users/user/Documents/CMC/prac4/server/server/mjs
<SCRIPT LANGUAGE="mjs" RUNAT="server">
{
	/*functions*/
	function x2(q)
	{
		return 2*q;
	}
	function x(y, z)
	{
		write(x2(4), " ");
		return x2(y)+z;
	}
	write(x(2,3), "\n");
}
</SCRIPT>