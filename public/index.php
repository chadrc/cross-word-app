<?hh
require_once("../src/globals.php");
require_once("../vendor/autoload.php");
require_once("../src/database/Impl.hh");
require_once("../src/elements/router/router.hh");
require_once("../src/elements/pages/notfound-page.hh");
require_once("../src/elements/pages/index-page.hh");
require_once("../src/elements/pages/puzzle-page.hh");
require_once("../src/elements/pages/create-page.hh");
require_once("../src/elements/pages/puzzles-page.hh");
require_once("../src/elements/pages/puzzle-solve.hh");

DB()->recordRequest(fetchPOST(), fetchGET(), requestUri());

if (requestUri() === "/puzzle/solve") {
  echo PuzzleSolve();
} else {
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
      <route match="/puzzles">
        <puzzles-page />
      </route>
    </router>
  );
}
