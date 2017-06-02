<?hh
require_once("../shared/page.hh");
require_once("../shared/requests.php");
require_once("../shared/create.hh");
require_once("../shared/make.hh");

if (requestUri() === "/create") {
  echo create_route();
} else if (requestUri() === "/make") {
  echo make_route();
} else {
  echo (
  <page title="Cross Word App">
    <h1>Cross Word App</h1>
    <a href="create">Create</a>
  </page>
  );
}
