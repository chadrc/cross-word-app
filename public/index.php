<?hh
require_once("../vendor/autoload.php");
require_once("../shared/Database.hh");
require_once("../shared/requests.php");
require_once("../shared/make.hh");
require_once("../shared/pages/IndexPage.hh");
require_once("../shared/pages/PuzzlePage.hh");
require_once("../shared/pages/CreatePage.hh");

recordRequest(fetchPOST(), fetchGET(), requestUri());

// if (requestUri() === "/info") {
//   echo (
//     <page title="Php info">
//       {var_dump(phpinfo())}
//     </page>
//   );
// } else

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
  } else if ($real_parts[0] === "make") {
    echo make_route();
  } else if ($real_parts[0] === "puzzle") {
    echo <puzzle-page path-params={$real_parts} />;
  }
} else {
  echo <index-page path-params={$real_parts} />;
}
