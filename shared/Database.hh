<?hh
require_once("../shared/cross-word/cross-word.hh");

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
  return $store;
}

function getCrossWord(string $id): ?CrossWord {
  $store = database()->CrossWords->findOne(['_id' => new MongoDB\BSON\ObjectID($id)]);
  if ($store === null) {
    return null;
  }
  $grid = new CrossWordGrid();
  $strings = Vector {};
  foreach ($store->words as $value) {
    $str = new CrossWordString($value->word, $value->hint);
    $strings[] = $str;
    $cells = $str->get_cells();
    for ($i=0; $i<$cells->count(); $i++) {
      $val_cell = $value->cells[$i];
      $cell = $cells[$i];

      $cell->set_position($val_cell->x, $val_cell->y);

      $existing = $grid->get_cell($cell->get_x(), $cell->get_y());
      if ($existing === null) {
        $grid->set_cell($cell->get_x(), $cell->get_y(), $cell);
      } else {
        $existing->join($cell);
        $cell->join($existing);
      }
    }
  }
  return new CrossWord($strings, $grid);
}

class CrossWordStore {
  public MongoDB\BSON\ObjectID $_id;
  public array $words;
  public MongoDB\BSON\UTCDateTime $created;

  public function __construct(CrossWord $cross_word) {
    $this->_id = new MongoDB\BSON\ObjectID();
    $words = $cross_word->get_words();
    $this->words = [];
    $this->created = new MongoDB\BSON\UTCDateTime();
    foreach ($words as $word) {
      $this->words[] = new CrossWordStringStore($word);
    }
  }

  public function get_id() {
    return $this->_id->__toString();
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
