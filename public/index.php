<?hh
require_once("../src/globals.php");
require_once("../vendor/autoload.php");
require_once("../src/database/Impl.hh");
require_once("../src/pages/IndexPage.hh");
require_once("../src/pages/PuzzlePage.hh");
require_once("../src/pages/CreatePage.hh");

DB()->recordRequest(fetchPOST(), fetchGET(), requestUri());

$request_parts = new Vector(explode("/", requestUri()));
$real_parts = Vector {};
foreach ($request_parts as $value) {
  if ($value !== "") {
    $real_parts[] = $value;
  }
}
if ($real_parts->containsKey(0)) {
  if ($real_parts[0] === "create") {
    echo <create-page path-params={$real_parts} />;
  } else if ($real_parts[0] === "puzzle") {
    echo <puzzle-page path-params={$real_parts} />;
  }
} else {
  echo <index-page path-params={$real_parts} />;
}
