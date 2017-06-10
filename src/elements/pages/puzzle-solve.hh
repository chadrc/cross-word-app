<?hh
require_once("../src/elements/pages/base-page.hh");
require_once("../src/elements/content.hh");

function PuzzleSolve() {
  $data = new Map((array) json_decode(fetchPOSTBody()));
  $puzzleId = $data["puzzleId"];
  if ($puzzleId === null) {
    http_response_code(400);
    return json_encode(Map {
      "message" => "Puzzle id required."
    });
  }

  $answers = new Map((array)$data["answers"]);
  if ($answers === null) {
    http_response_code(400);
    return json_encode(Map {
      "message" => "Answers required"
    });
  }

  foreach ($answers as $key => $value) {
    $answers[$key] = new Map((array) $value);
  }

  $crossWord = DB()->getCrossWord($puzzleId);
  if ($crossWord === null) {
    http_response_code(404);
    return json_encode(Map {
      "message" => "Puzzle with id '$puzzleId' not found"
    });
  }

  $audits = Map {};
  $allCorrect = true;
  $cells = $crossWord->get_grid()->get_cells();
  foreach($cells as $cell) {
    $x = $cell->get_x() . "";
    $y = $cell->get_y() . "";
    if (!$audits->containsKey($y)) {
      $audits[$y] = Map {};
    }

    if (!$answers->containsKey($y) || !$answers[$y]->containsKey($x)) {
      $audits[$y][$x] = new CellAudit(true, false);
      continue;
    }

    $answer = $answers[$y][$x];
    $correct = $answer === $cell->get_letter();
    if (!$correct) {
      $allCorrect = false;
    }
    $audits[$y][$x] = new CellAudit(false, $correct);
  }

  $response = Map {};
  $response['answers'] = $audits;
  $response['solved'] = $allCorrect;
  return json_encode($response);
}

class CellAudit {
  public function __construct(public bool $missing, public bool $correct) {}
}
