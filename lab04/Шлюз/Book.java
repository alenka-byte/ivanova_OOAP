public class Book {
    private int id;
    private String title;
    private String author;
    private int year;
    private int mark;

    public Book() {}

    public Book(String title, String author, int year, int mark) {
        this.title = title;
        this.author = author;
        this.year = year;
        this.mark = mark;
    }

    public int getId() { return id; }
    public void setId(int id) { this.id = id; }

    public String getTitle() { return title; }
    public void setTitle(String title) { this.title = title; }

    public String getAuthor() { return author; }
    public void setAuthor(String author) { this.author = author; }

    public int getYear() { return year; }
    public void setYear(int year) { this.year = year; }

    public int getMark() { return mark; }
    public void setMark(int mark) { this.mark = mark; }

    @Override
    public String toString() {
        return String.format("""
                ┌─────────────── КНИГА ───────────────┐
                │ Название: %-20s │
                │ Автор:   %-20s │
                │ Год:     %-20d │
                │ Оценка:  %-20d │
                └─────────────────────────────────────┘""",
                title, author, year, mark);
    }
}