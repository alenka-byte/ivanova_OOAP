import java.util.List;

public interface LibraryGateway {
    /**
     * Поиск книг по строке (по названию или автору) с учётом сортировки.
     * @param keyword поисковая фраза
     * @param orderBy строка ORDER BY (например "title", "mark ASC", "mark DESC")
     * @return список найденных книг
     */
    List<Book> searchBooks(String keyword, String orderBy);
    
    /**
     * Получить все книги с заданной сортировкой.
     * @param orderBy строка ORDER BY
     * @return список всех книг
     */
    List<Book> getAllBooks(String orderBy);
    
    /**
     * Добавить книгу в базу.
     * @param book объект книги (без id)
     * @return true если успешно
     */
    boolean addBook(Book book);
}