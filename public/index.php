<?hh
require_once("../src/globals.php");
require_once("../vendor/autoload.php");
require_once("../src/database/Impl.hh");
require_once("../src/elements/router/router.hh");
require_once("../src/elements/IndexPage.hh");
require_once("../src/elements/PuzzlePage.hh");
require_once("../src/elements/CreatePage.hh");

DB()->recordRequest(fetchPOST(), fetchGET(), requestUri());

$request_parts = new Vector(explode("/", requestUri()));
$real_parts = Vector {};
foreach ($request_parts as $value) {
  if ($value !== "") {
    $real_parts[] = $value;
  }
}

echo (
  <router path={requestUri()}>
    <route match="/">
      <index-page path-params={$real_parts}></index-page>
    </route>
    <route match="/create">
      <create-page path-params={$real_parts}></create-page>
    </route>
    <route match="/puzzle">
      <puzzle-page path-params={$real_parts}></puzzle-page>
    </route>
  </router>
);

// if ($real_parts->containsKey(0)) {
//   if ($real_parts[0] === "create") {
//     echo <create-page path-params={$real_parts} />;
//   } else if ($real_parts[0] === "puzzle") {
//     echo <puzzle-page path-params={$real_parts} />;
//   }
// } else {
//   echo <index-page path-params={$real_parts} />;
// }
