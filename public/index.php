<?hh
require_once("../shared/Database.hh");
require_once("../shared/page.hh");
require_once("../shared/requests.php");
require_once("../shared/create.hh");
require_once("../shared/make.hh");

recordRequest(fetchPOST(), fetchGET(), requestUri());

// if (requestUri() === "/info") {
//   echo (
//     <page title="Php info">
//       {var_dump(phpinfo())}
//     </page>
//   );
// } else
if (requestUri() === "/create") {
  echo create_route();
} else if (requestUri() === "/make") {
  echo make_route();
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
