<?hh
require_once("../src/globals.php");
require_once("../vendor/autoload.php");
require_once("../src/database/Impl.hh");
require_once("../src/elements/router/router.hh");
require_once("../src/elements/pages/404Page.hh");
require_once("../src/elements/pages/IndexPage.hh");
require_once("../src/elements/pages/PuzzlePage.hh");
require_once("../src/elements/pages/CreatePage.hh");

DB()->recordRequest(fetchPOST(), fetchGET(), requestUri());

echo (
  <router>
    <route match="/">
      <index-page />
    </route>
    <route match="/create">
      <create-page />
    </route>
    <route match="/puzzle/[a-z 0-9]{24}">
      <puzzle-page />
    </route>
  </router>
);
