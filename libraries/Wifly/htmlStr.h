char htmlHead[] = "<html>"
	"<head>"
		"<title>STM32 테스트 페이지</title>"
	"</head>"
	"<script>"
	"function uartout(){"
		"document.UARTOUT.action='index.html?uart=' + document.UARTOUT.uart.value;"
		"document.UARTOUT.submit();"
	"}"
	"</script>"
	"<body>"
		"<center><table width=\"400px\">"
			"<tr>"
				"<td colspan=\"2\">"
					"<big><strong>EWCK-STM32 테스트 페이지</strong></big>"
				"</td>"
			"</tr>"
			"<tr>"
                   "!%led_control "
			"</tr>"
                          "<tr></tr>"
			"<tr>"
				"<td colspan=\"2\">"
					"!%uart_control "
				"</td>"
			"</tr>"
			"<tr>"
				"<td colspan=\"2\">"
                                      "<iframe frameborder=\"0\" src=\"timer.html\" width=\"300\" height=\"100\"></iframe>"
				"</td>"
			"</tr>"

		"</table></center>"
        "</body>"
"</html>";
char htmlBody[] = "<body><h3>Function</h3>Example for Simple Function.<br>Drawing table has two row construct to function.Let's call function.<script language=\"javascript\">document.bgColor=\"gray\";myFunc('blue', 'green');</script><br>Calling the function.Retry.<script language=\"javascript\">myFunc('red', 'gold');</script><br>Do you want call again?Let's call again.<script language=\"javascript\">myFunc(\"blue\", \"gray\");</script></body></html>";