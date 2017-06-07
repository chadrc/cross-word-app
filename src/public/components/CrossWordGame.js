class CrossWordGame extends React.Component {
  constructor(props) {
    super(props);
  }

  render() {
    return (
      <section>
        <h1>Solve</h1>
        <section className="cross-word">
        </section>
        <section className="word-hints">
        </section>
      </section>
    );
  }
}

window.CrossWordGame = CrossWordGame;
