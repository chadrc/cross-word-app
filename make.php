<?hh
require_once("vendor/facebook/xhp-lib/init.php");
require_once("requests.php");

$words = postString("words");
$hints = postString("hints");

echo (
<html>
<head>
  <title>Cross Word App</title>
</head>
<body>
  <p>Words: {$words}</p>
  <p>Hints: {$hints}</p>
</body>
</html>);
