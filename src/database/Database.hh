<?hh

interface Database {
  public function recordRequest(array $postParams, array $getParams, string $uri);
  public function saveCrossWord(CrossWord $cross_word);
  public function getCrossWord(string $id): ?CrossWord;
}
