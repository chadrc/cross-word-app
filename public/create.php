<?hh
require_once("../shared/page.hh");

echo (
<page title="Create a Cross Word">
  <h1>Create Cross Word</h1>
  <form action="make.php" method="post">
    <label for="words">Words (Comma separated)</label>
    <textarea id="words" name="words" />

    <label for="hints">Hints (Comma separated)</label>
    <textarea id="hints" name="hints" />

    <button type="submit">Make</button>
  </form>
</page>
);
