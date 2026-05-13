import java.sql.*;
import java.util.ArrayList;
import java.util.List;

public class MySQLBookGateway implements LibraryGateway {
    private final String url;
    private final String user;
    private final String password;

    public MySQLBookGateway(String url, String user, String password) {
        this.url = url;
        this.user = user;
        this.password = password;
        try {
            Class.forName("com.mysql.cj.jdbc.Driver");
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        }
    }

    @Override
    public List<Book> searchBooks(String keyword, String orderBy) {
        List<Book> books = new ArrayList<>();
        if (keyword == null || keyword.trim().isEmpty()) return books;
        
        String likeKeyword = "%" + keyword + "%";
        String sql = "SELECT id, title, author, year, mark FROM books " +
                     "WHERE LOWER(title) LIKE LOWER(?) OR LOWER(author) LIKE LOWER(?) " +
                     "ORDER BY " + orderBy;
        
        try (Connection conn = DriverManager.getConnection(url, user, password);
             PreparedStatement stmt = conn.prepareStatement(sql)) {
            stmt.setString(1, likeKeyword);
            stmt.setString(2, likeKeyword);
            try (ResultSet rs = stmt.executeQuery()) {
                while (rs.next()) {
                    Book book = new Book();
                    book.setId(rs.getInt("id"));
                    book.setTitle(rs.getString("title"));
                    book.setAuthor(rs.getString("author"));
                    book.setYear(rs.getInt("year"));
                    book.setMark(rs.getInt("mark"));
                    books.add(book);
                }
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return books;
    }

    @Override
    public List<Book> getAllBooks(String orderBy) {
        List<Book> books = new ArrayList<>();
        String sql = "SELECT id, title, author, year, mark FROM books ORDER BY " + orderBy;
        try (Connection conn = DriverManager.getConnection(url, user, password);
             Statement stmt = conn.createStatement();
             ResultSet rs = stmt.executeQuery(sql)) {
            while (rs.next()) {
                Book book = new Book();
                book.setId(rs.getInt("id"));
                book.setTitle(rs.getString("title"));
                book.setAuthor(rs.getString("author"));
                book.setYear(rs.getInt("year"));
                book.setMark(rs.getInt("mark"));
                books.add(book);
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return books;
    }

    @Override
    public boolean addBook(Book book) {
        String sql = "INSERT INTO books (title, author, year, mark) VALUES (?, ?, ?, ?)";
        try (Connection conn = DriverManager.getConnection(url, user, password);
             PreparedStatement stmt = conn.prepareStatement(sql)) {
            stmt.setString(1, book.getTitle());
            stmt.setString(2, book.getAuthor());
            stmt.setInt(3, book.getYear());
            stmt.setInt(4, book.getMark());
            return stmt.executeUpdate() > 0;
        } catch (SQLException e) {
            e.printStackTrace();
            return false;
        }
    }
}