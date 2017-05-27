<?hh
require_once("shared/page.hh");
require_once("shared/requests.php");

$words = postString("words");
$hints = postString("hints");

echo (
<page>
  <p>Words: {$words}</p>
  <p>Hints: {$hints}</p>
</page>
);
