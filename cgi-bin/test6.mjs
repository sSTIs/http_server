#! /Users/user/Documents/CMC/prac4/server/server/mjs
<SCRIPT LANGUAGE="mjs" RUNAT="server">
{
	/*expressions*/
	var x =3;
	write(x,"\n");
	x = 4+10%3;
	x = (x + 1) * (3+5*2);
	write(x, "\n");
	write(x++, "\n");
	write(x, "\n");
	x = x + 1 && x + 1 || ++x;
	write(x, "\n");
}
</SCRIPT>