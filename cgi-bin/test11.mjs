#! /Users/user/Documents/CMC/prac4/server/server/mjs
<SCRIPT LANGUAGE="mjs" RUNAT="server">
{
	/*functions2*/
	function b()
	{
		write("b");
	}
	function a()
	{
		write("a");
		b();
		b();
	}

	a();
	b();
	a();
	b();
	b();
	a();
}
</SCRIPT>