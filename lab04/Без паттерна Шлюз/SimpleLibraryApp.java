import javax.swing.*;
import javax.swing.border.EmptyBorder;
import java.awt.*;
import java.sql.*;
import java.util.ArrayList;
import java.util.List;

public class SimpleLibraryApp extends JFrame {
    // Параметры подключения к БД
    private static final String DB_URL = "jdbc:mysql://localhost/lib?serverTimezone=Europe/Moscow&useSSL=false&allowPublicKeyRetrieval=true";
    private static final String DB_USER = "root";      
    private static final String DB_PASSWORD = "Cjkysirj1!"; 

    private JTextField searchField;
    private JTextArea resultArea;
    private JButton searchButton;
    private JComboBox<String> sortCombo; // выпадающий список для сортировки

    public SimpleLibraryApp() {
        setTitle("Моя библиотека");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(650, 550);
        setLocationRelativeTo(null);
        setLayout(new BorderLayout(10, 10));
        ((JPanel) getContentPane()).setBorder(new EmptyBorder(10, 10, 10, 10));

        // Верхняя панель: поле ввода + кнопка + сортировка
        JPanel topPanel = new JPanel(new BorderLayout(5, 5));
        
        // Строка поиска
        JPanel inputPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        inputPanel.add(new JLabel("Поиск:"));
        searchField = new JTextField(25);
        inputPanel.add(searchField);
        searchButton = new JButton("🔍 Найти");
        inputPanel.add(searchButton);
        topPanel.add(inputPanel, BorderLayout.NORTH);
        
        // Панель сортировки
        JPanel sortPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        sortPanel.add(new JLabel("Сортировка:"));
        String[] sortOptions = {"По названию (А-Я)", "По оценке (возрастание)", "По оценке (убывание)"};
        sortCombo = new JComboBox<>(sortOptions);
        sortCombo.addActionListener(e -> performSearch()); // при смене сортировки сразу обновляем
        sortPanel.add(sortCombo);
        topPanel.add(sortPanel, BorderLayout.CENTER);
        
        add(topPanel, BorderLayout.NORTH);

        // Область вывода результатов
        resultArea = new JTextArea();
        resultArea.setEditable(false);
        resultArea.setFont(new Font("Monospaced", Font.PLAIN, 13));
        resultArea.setBackground(new Color(250, 250, 250));
        JScrollPane scroll = new JScrollPane(resultArea);
        scroll.setBorder(BorderFactory.createTitledBorder("Результаты поиска"));
        add(scroll, BorderLayout.CENTER);

        // Нижняя панель: кнопка добавления
        JPanel bottomPanel = new JPanel();
        JButton addButton = new JButton("➕ Добавить новую книгу");
        bottomPanel.add(addButton);
        add(bottomPanel, BorderLayout.SOUTH);

        // Обработчики
        searchButton.addActionListener(e -> performSearch());
        searchField.addActionListener(e -> performSearch());
        addButton.addActionListener(e -> showAddBookDialog());

        setVisible(true);
    }

    // Возвращает строку ORDER BY в зависимости от выбранного пункта сортировки
    private String getOrderByClause() {
        int selectedIndex = sortCombo.getSelectedIndex();
        switch (selectedIndex) {
            case 0: return "ORDER BY title";
            case 1: return "ORDER BY mark ASC";
            case 2: return "ORDER BY mark DESC";
            default: return "ORDER BY title";
        }
    }

    private void performSearch() {
        String keyword = searchField.getText().trim();

        // Если поле пусто – показать все книги с учётом сортировки
        if (keyword.isEmpty()) {
            showAllBooks();
            return;
        }

        // Поиск по названию или автору с учётом выбранной сортировки
        SwingWorker<List<Object[]>, Void> worker = new SwingWorker<List<Object[]>, Void>() {
            @Override
            protected List<Object[]> doInBackground() {
                return searchBooks(keyword);
            }
            @Override
            protected void done() {
                try {
                    List<Object[]> books = get();
                    if (books.isEmpty()) {
                        resultArea.setText("Книги не найдены.\nПопробуйте другой запрос или добавьте книгу.");
                    } else {
                        StringBuilder sb = new StringBuilder();
                        sb.append("Найдено книг: ").append(books.size()).append("\n\n");
                        for (Object[] b : books) {
                            sb.append(formatBook(b)).append("\n\n");
                        }
                        resultArea.setText(sb.toString());
                        resultArea.setCaretPosition(0);
                    }
                } catch (Exception ex) {
                    resultArea.setText("Ошибка: " + ex.getMessage());
                    ex.printStackTrace();
                } finally {
                    searchButton.setEnabled(true);
                }
            }
        };
        searchButton.setEnabled(false);
        worker.execute();
    }

    private List<Object[]> searchBooks(String keyword) {
        List<Object[]> books = new ArrayList<>();
        String orderBy = getOrderByClause();
        String sql = "SELECT id, title, author, year, mark FROM books WHERE LOWER(title) LIKE LOWER(?) OR LOWER(author) LIKE LOWER(?) " + orderBy;
        String likeKeyword = "%" + keyword + "%";
        try (Connection conn = DriverManager.getConnection(DB_URL, DB_USER, DB_PASSWORD);
             PreparedStatement stmt = conn.prepareStatement(sql)) {
            stmt.setString(1, likeKeyword);
            stmt.setString(2, likeKeyword);
            try (ResultSet rs = stmt.executeQuery()) {
                while (rs.next()) {
                    books.add(new Object[]{
                        rs.getInt("id"),
                        rs.getString("title"),
                        rs.getString("author"),
                        rs.getInt("year"),
                        rs.getInt("mark")
                    });
                }
            }
        } catch (SQLException e) {
            e.printStackTrace();
            resultArea.setText("Ошибка БД: " + e.getMessage());
        }
        return books;
    }

    private void showAllBooks() {
        SwingWorker<List<Object[]>, Void> worker = new SwingWorker<List<Object[]>, Void>() {
            @Override
            protected List<Object[]> doInBackground() {
                return getAllBooks();
            }
            @Override
            protected void done() {
                try {
                    List<Object[]> books = get();
                    if (books.isEmpty()) {
                        resultArea.setText("Библиотека пуста. Добавьте книги.");
                    } else {
                        StringBuilder sb = new StringBuilder();
                        sb.append("Все книги (с учётом сортировки):\n\n");
                        for (Object[] b : books) {
                            sb.append(formatBook(b)).append("\n\n");
                        }
                        resultArea.setText(sb.toString());
                        resultArea.setCaretPosition(0);
                    }
                } catch (Exception ex) {
                    resultArea.setText("Ошибка: " + ex.getMessage());
                } finally {
                    searchButton.setEnabled(true);
                }
            }
        };
        searchButton.setEnabled(false);
        worker.execute();
    }

    private List<Object[]> getAllBooks() {
        List<Object[]> books = new ArrayList<>();
        String orderBy = getOrderByClause();
        String sql = "SELECT id, title, author, year, mark FROM books " + orderBy;
        try (Connection conn = DriverManager.getConnection(DB_URL, DB_USER, DB_PASSWORD);
             Statement stmt = conn.createStatement();
             ResultSet rs = stmt.executeQuery(sql)) {
            while (rs.next()) {
                books.add(new Object[]{
                    rs.getInt("id"),
                    rs.getString("title"),
                    rs.getString("author"),
                    rs.getInt("year"),
                    rs.getInt("mark")
                });
            }
        } catch (SQLException e) {
            e.printStackTrace();
            resultArea.setText("Ошибка БД: " + e.getMessage());
        }
        return books;
    }

    private void addBook(String title, String author, int year, int mark) {
        String sql = "INSERT INTO books (title, author, year, mark) VALUES (?, ?, ?, ?)";
        try (Connection conn = DriverManager.getConnection(DB_URL, DB_USER, DB_PASSWORD);
             PreparedStatement stmt = conn.prepareStatement(sql)) {
            stmt.setString(1, title);
            stmt.setString(2, author);
            stmt.setInt(3, year);
            stmt.setInt(4, mark);
            stmt.executeUpdate();
        } catch (SQLException e) {
            e.printStackTrace();
            JOptionPane.showMessageDialog(this, "Ошибка добавления: " + e.getMessage(), "Ошибка", JOptionPane.ERROR_MESSAGE);
        }
    }

    private void showAddBookDialog() {
        JPanel panel = new JPanel(new GridLayout(0, 2, 5, 5));
        panel.add(new JLabel("Название:"));
        JTextField titleField = new JTextField(20);
        panel.add(titleField);
        panel.add(new JLabel("Автор:"));
        JTextField authorField = new JTextField(20);
        panel.add(authorField);
        panel.add(new JLabel("Год издания:"));
        JTextField yearField = new JTextField(20);
        panel.add(yearField);
        panel.add(new JLabel("Оценка (1-10):"));
        JTextField markField = new JTextField(20);
        panel.add(markField);

        Object[] options = { "Добавить", "Отмена" };
        int result = JOptionPane.showOptionDialog(this, panel,
                "Добавление новой книги",
                JOptionPane.OK_CANCEL_OPTION,
                JOptionPane.PLAIN_MESSAGE,
                null,
                options,
                options[0]);

        if (result == JOptionPane.OK_OPTION) {
            String title = titleField.getText().trim();
            String author = authorField.getText().trim();
            String yearStr = yearField.getText().trim();
            String markStr = markField.getText().trim();

            if (title.isEmpty() || author.isEmpty() || yearStr.isEmpty() || markStr.isEmpty()) {
                JOptionPane.showMessageDialog(this, "Заполните все поля", "Ошибка", JOptionPane.ERROR_MESSAGE);
                return;
            }
            int year, mark;
            try {
                year = Integer.parseInt(yearStr);
                mark = Integer.parseInt(markStr);
                if (mark < 1 || mark > 10) throw new NumberFormatException();
            } catch (NumberFormatException e) {
                JOptionPane.showMessageDialog(this, "Год – число, оценка от 1 до 10", "Ошибка", JOptionPane.ERROR_MESSAGE);
                return;
            }

            addBook(title, author, year, mark);
            JOptionPane.showMessageDialog(this, "Книга добавлена!", "Успех", JOptionPane.INFORMATION_MESSAGE);
            performSearch(); 
        }
    }

    private String formatBook(Object[] book) {
        String title = (String) book[1];
        String author = (String) book[2];
        int year = (int) book[3];
        int mark = (int) book[4];
        return String.format("""
                ┌─────────────── КНИГА ───────────────┐
                │ Название: %-20s │
                │ Автор:   %-20s │
                │ Год:     %-20d │
                │ Оценка:  %-20d │
                └─────────────────────────────────────┘""",
                title, author, year, mark);
    }

    public static void main(String[] args) {
        // Загружаем драйвер MySQL
        try {
            Class.forName("com.mysql.cj.jdbc.Driver");
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
            JOptionPane.showMessageDialog(null, "MySQL JDBC Driver не найден!\nДобавьте JAR в classpath.", "Ошибка", JOptionPane.ERROR_MESSAGE);
            System.exit(1);
        }
        SwingUtilities.invokeLater(SimpleLibraryApp::new);
    }
}