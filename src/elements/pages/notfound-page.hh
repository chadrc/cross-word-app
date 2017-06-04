<?hh

class :not-found extends :base-page {
  attribute string message;
  protected function render(): XHPRoot {
    $message = "Sorry, the information you requested could not be found.";
    if ($this->:message !== null) {
      $message = $this->:message;
    }
    return (
      <main>
        <h1>Not Found</h1>
        <p>{$message}</p>
      </main>
    );
  }
}
