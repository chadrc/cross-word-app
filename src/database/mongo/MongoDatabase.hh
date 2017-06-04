<?hh
require_once("../src/cross-word/cross-word.hh");
require_once("../src/database/Database.hh");
require_once("../src/database/mongo/MongoCrossWordStore.hh");

class MongoDatabase implements Database {
  private MongoDB\Collection $crossword;
  private MongoDB\Collection $requestlog;

  public function __construct(){
    $db = (new MongoDB\Client("mongodb://172.17.0.1:27017"))->CrossWordApp;
    $this->crossword = $db->CrossWord;
    $this->requestlog = $db->RequestLog;
  }

  public function recordRequest(array $postParams, array $getParams, string $uri) {
    $this->requestlog->insertOne([
      'postParams' => fetchPOST(),
      'getParams' => fetchGET(),
      'uri' => requestUri(),
      'created' => new MongoDB\BSON\UTCDateTime()
    ]);
  }

  public function saveCrossWord(CrossWord $cross_word) {
    $store = new CrossWordStore($cross_word);
    $this->crossword->insertOne($store);
    return $store;
  }

  public function getCrossWord(string $id): ?CrossWord {
    $store = $this->crossword->findOne(['_id' => new MongoDB\BSON\ObjectID($id)]);
    return $this->storeToCrossWord($store);
  }

  public function getCrossWords(int $count, int $page): Page<CrossWord> {
    $cursor = $this->crossword->find(
      [],
      [
          'limit' => $count,
          'skip' => ($page * $count),
          'sort' => ['created' => -1],
      ]
    );

    $crosswords = Vector {};
    foreach ($cursor as $document) {
      $cw = $this->storeToCrossWord($document);
      if ($cw !== null) {
        $crosswords[] = $cw;
      }
    }

    $total = $this->crossword->count();

    return new Page<CrossWord>($crosswords, $total);
  }

  private function storeToCrossWord(MongoDB\Model\BSONDocument $store): ?CrossWord {
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
    $cw = new CrossWord($strings, $grid);
    $cw->set_id($store->_id . "");
    $cw->set_created($store->created->toDateTime());
    return $cw;
  }
}
