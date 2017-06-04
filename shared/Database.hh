<?hh
require_once("../shared/cross-word.hh");

function database(): MongoDB\Database {
  return (new MongoDB\Client("mongodb://172.17.0.1:27017"))->CrossWordApp;
}

function recordRequest(array $postParams, array $getParams, string $uri) {
  $collection = database()->RequestLog;
  $collection->insertOne([
    'postParams' => fetchPOST(),
    'getParams' => fetchGET(),
    'uri' => requestUri(),
    'created' => new MongoDB\BSON\UTCDateTime()
  ]);
}

function saveCrossWord(CrossWord $cross_word) {
  $store = new CrossWordStore($cross_word);
  $collection = database()->CrossWords;
  $collection->insertOne($store);
}

class CrossWordStore {
  public array $words;
  public MongoDB\BSON\UTCDateTime $created;

  public function __construct(CrossWord $cross_word) {
    $words = $cross_word->get_words();
    $this->words = [];
    $this->created = new MongoDB\BSON\UTCDateTime();
    foreach ($words as $word) {
      $this->words[] = new CrossWordStringStore($word);
    }
  }
}

class CrossWordStringStore {
  public array $cells;
  public string $word;
  public string $hint;

  public function __construct(CrossWordString $string) {
    $this->word = $string->get_word();
    $this->hint = $string->get_hint();
    $this->cells = [];
    foreach ($string->get_cells() as $cell) {
      $this->cells[] = new CrossWordCellStore($cell);
    }
  }
}

class CrossWordCellStore {
  public string $letter;
  public int $x;
  public int $y;

  public function __construct(CrossWordCell $cell) {
    $this->letter = $cell->get_letter();
    $this->x = $cell->get_x();
    $this->y = $cell->get_y();
  }
}
