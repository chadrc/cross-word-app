<?hh
require_once("../src/globals.php");
require_once("../vendor/autoload.php");
require_once("../src/database/Impl.hh");
require_once("../src/elements/router/router.hh");
require_once("../src/elements/IndexPage.hh");
require_once("../src/elements/PuzzlePage.hh");
require_once("../src/elements/CreatePage.hh");

DB()->recordRequest(fetchPOST(), fetchGET(), requestUri());

echo (
  <router>
    <route match="/">
      <index-page />
    </route>
    <route match="/create">
      <create-page />
    </route>
    <route match="/puzzle">
      <puzzle-page />
    </route>
  </router>
);
