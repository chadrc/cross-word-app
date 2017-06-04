<?hh
require_once("../shared/Database.hh");
require_once("../shared/page.hh");
require_once("../shared/requests.php");
require_once("../shared/create.hh");
require_once("../shared/make.hh");
require_once("../shared/pages/PuzzlePage.hh");

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
    echo create_route();
  } else if ($real_parts[0] === "make") {
    echo make_route();
  } else if ($real_parts[0] === "puzzle") {
    echo <puzzle-page path_params={$real_parts} />;
  }
} else {
  echo (
    <page title="Cross Word App">
      <h1>Cross Word App</h1>
      <nav>
        <ul>
          <li><a href="create">Create</a></li>
        </ul>
      </nav>
    </page>
  );
}
