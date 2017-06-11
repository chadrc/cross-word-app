class CrossWordForm extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      title: "",
      words: []
    };
  }

  render() {
    return (
      <form>
        <section>
          <label htmlFor="title">Title</label>
          <input tabIndex={1} type="text" id="title" />
        </section>
        <section>
          <label>Words</label>
          {this.state.words.map((item, index) => {
            return <WordInput tabIndex={index * 2 + 2} />
          })}
          <WordInput tabIndex={this.state.words.length * 2 + 2} />
        </section>
      </form>
    );
  }
}

window.CrossWordForm = CrossWordForm;

class WordInput extends React.Component {
  constructor(props) {
    super(props);
  }

  raiseWordChanged(value) {

  }

  raiseHintChanged(value) {

  }

  render() {
    return (
      <div className="word-input">
        <input tabIndex={this.props.tabIndex} type="text" onChange={(e) => this.raiseWordChanged(e.target.value)} />
        <textarea tabIndex={this.props.tabIndex + 1} rows={2} type="text" onChange={(e) => this.raiseHintChanged(e.target.value)} />
      </div>
    );
  }
}
