<?hh
require_once("vendor/facebook/xhp-lib/init.php");

echo (
<html>
<head>
  <title>Cross Word App</title>
</head>
<body>
  <h1>Create Cross Word</h1>
  <form action="make.php" method="post">
    <label for="words">Words (Comma separated)</label>
    <textarea id="words" name="words" />

    <label for="hints">Hints (Comma separated)</label>
    <textarea id="hints" name="hints" />

    <button type="submit">Make</button>
  </form>
</body>
</html>);
