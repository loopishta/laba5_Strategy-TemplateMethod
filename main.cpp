#include <iostream>
 #include <vector>
 #include <list>
 #include <stdio.h>
 #include <stdlib.h>
 #include "PT2.h"

 using namespace std;

 enum class BookGenre : int
 {
     Fiction,
     Biography,
     Science,
     Unknown
 };

 class Book
 {
 private:
     BookGenre Genre;
     int Pages;
 protected:
     bool BookIsPopular;
     bool BookIsBig;
 public:
     Book(BookGenre genre, int pages)
         : Genre(genre), Pages(pages)
     {
         // Значение инициализируется случайным числом 0 или 1
         BookIsPopular = rand() % 2;
     }

     virtual ~Book() {}

     BookGenre GetGenre() const { return Genre; }
     bool IsPopular() const { return BookIsPopular; }
     bool IsBig() const {return BookIsBig; }
     int GetPages() const { return Pages; }

     virtual void Read()
     {
         if (IsPopular())
         {
             cout << "Reading POPULAR book... ";
         }
         if (!IsPopular())
         {
             cout << "Reading LESS-KNOWN book... ";
         }

         if (GetPages() > 300)
         {
             BookIsBig = true;
             cout << "Reading BIG book... ";
         }
         if (!(GetPages() > 300))
         {
             BookIsBig = false;
             cout << "Reading SMALL book... ";
         }
     }
 };

 class Novel : public Book
 {
 public:
     Novel();
     virtual ~Novel() {}
     // Переопределение функции в унаследованном классе (функция виртуальная!)
     void Read() override;
 };
 // Реализация конструктора
 Novel::Novel() : Book(BookGenre::Fiction, 300) {}

 void Novel::Read()
 {
     Book::Read();
     cout << "Reading a novel..." << endl;
 }

 class Biography : public Book {
 public:
     Biography() : Book(BookGenre::Biography, 250) {}
     virtual ~Biography() {}

     void Read() override;
 };

 void Biography::Read()
 {
     Book::Read();
     cout << "Reading a biography..." << endl;
 }

 class Encyclopedia: public Book
 {
 public:
     Encyclopedia() : Book(BookGenre::Science, 800) {}
     virtual ~Encyclopedia() {}

     void Read() override;
 };

 void Encyclopedia::Read()
 {
     Book::Read();
     cout << "Reading an Encyclopedia..." << endl;
 }

 enum class BookType {
     Novel = 1,
     Biography = 2,
     Science = 3,

     Undefined = 0
 };

 Book* CreateBook(BookType type)
 {
     if (type == BookType::Novel)
     {
         return new Novel;
     }
     else if (type == BookType::Biography)
     {
         return new Biography;
     }
     else if (type == BookType::Science)
     {
         return new Encyclopedia;
     }

     return nullptr;
 }
 ///
 // Декоратор итератора для выделения книг по жанру

 class BookGenreDecorator : public IteratorDecorator<class Book*>
 {
 private:
     BookGenre TargetGenre;

 public:
     BookGenreDecorator(Iterator<Book*> *it, BookGenre genre)
     : IteratorDecorator<Book*>(it), TargetGenre(genre) {}

     void First()
     {
         It->First();
         while(!It->IsDone() && It->GetCurrent()->GetGenre() != TargetGenre)
         {
             It->Next();
         }
     }

     void Next()
     {
         do
         {
             It->Next();

         } while(!It->IsDone() && It->GetCurrent()->GetGenre() != TargetGenre);
     }
 };

 // Декоратор итератора для выделения популярных книг

 class BookPopularDecorator : public IteratorDecorator<class Book*>
 {
 private:
     bool TargetPopular;

 public:
     BookPopularDecorator(Iterator<Book*> *it, bool isPopular)
     : IteratorDecorator<Book*>(it), TargetPopular(isPopular) {}

     void First()
     {
         It->First();
         while(!It->IsDone() && It->GetCurrent()->IsPopular() != TargetPopular)
         {
             It->Next();
         }
     }

     void Next()
     {
         do
         {
             It->Next();

         } while(!It->IsDone() && It->GetCurrent()->IsPopular() != TargetPopular);
     }
 };

 ///
 class BookSizeDecorator : public IteratorDecorator<Book*> {
 private:
     bool TargetBig;  // false = маленькие книги

 public:
     BookSizeDecorator(Iterator<Book*>* it, bool isBig)
         : IteratorDecorator<Book*>(it), TargetBig(isBig) {}

     void First()
     {
         It->First();
         while(!It->IsDone() && It->GetCurrent()->IsBig() != TargetBig)
         {
             It->Next();
         }
     }

     void Next()
     {
         do
         {
             It->Next();

         } while(!It->IsDone() && It->GetCurrent()->IsBig() != TargetBig);
     }

 private:
     bool IsBig(Book* book) const {
         if (TargetBig)
         {
             return book->GetPages() > 300;
         }
         else
         {
             return book->GetPages() <= 300;
         }
     }
 };
 ///

 void ReadAll(Iterator<Book*> *it)
 {
     for (it -> First(); !it -> IsDone(); it -> Next())
     {
         Book *currentBook = it -> GetCurrent();
         currentBook -> Read();
     }
 }

 int main()
 {
     setlocale(LC_ALL, "Russian");
     size_t N = 20;

     ArrayClass <Book*> BookArray;
     for(size_t i = 0; i < N; i++)
     {
         int book_num = rand()%3 + 1;
         BookType book_type = static_cast <BookType> (book_num);
         Book *newBook = CreateBook(book_type);
         BookArray.Add(newBook);
     }
     wcout << L"Размер массива книг: " << BookArray.Size() << endl;

     // Связанный список книг (для демонстрации адаптера)

     list <Book*> bookVector; // а еще можно vector, forward_list, ...
     for(size_t i=0; i<N; i++)
     {
         int book_num = rand()%3 + 1;
         BookType book_type = static_cast <BookType> (book_num);
         Book *newBook = CreateBook(book_type);
         bookVector.push_back(newBook); // Добавить в конец списка
     }

     wcout << L"Размер списка книг: " << bookVector.size() << endl;

     // Обход в простом цикле
     cout << endl << "Reading all in a simple way:" << endl;
     for(size_t i=0; i<BookArray.Size(); i++)
     {
         Book *currentBook = BookArray[i];
         currentBook->Read();
     }

     // Обход всех элементов при помощи итератора
     cout << endl << "Reading all using iterator:" << endl;
     Iterator<Book*> *allIt = BookArray.GetIterator();
     ReadAll(allIt);
     delete allIt;

     // Обход всех хороших фруктов
     cout << endl << "Reading all popular using iterator:" << endl;
     Iterator<Book*> *popIt = new BookPopularDecorator(BookArray.GetIterator(), true);
     ReadAll(popIt);
     delete popIt;

     cout << endl << "Reading all fiction using iterator:" << endl;
     Iterator<Book*> *fictionIt = new BookGenreDecorator(BookArray.GetIterator(), BookGenre::Fiction);
     ReadAll(fictionIt);
     delete fictionIt;

 ///
     cout << endl << "Reading all small books using iterator:" << endl;
     Iterator<Book*>* smallBooksIt =
         new BookSizeDecorator(BookArray.GetIterator(), false);  // false — только маленькие

     ReadAll(smallBooksIt);

     delete smallBooksIt;

 ///

     cout << endl << "Reading all popular biography using iterator:" << endl;
     Iterator<Book*> *popBiogIt =
         new BookPopularDecorator(new BookGenreDecorator(BookArray.GetIterator(), BookGenre::Biography), true);
     ReadAll(popBiogIt);
     delete popBiogIt;

     // Демонстрация работы адаптера
     cout << endl << "Reading all popular science using adapted iterator (another container):" << endl;
     Iterator<Book*> *adaptedIt = new ConstIteratorAdapter<list<Book*>, Book*>(&bookVector);
     Iterator<Book*> *adaptedScienceIt = new BookPopularDecorator(new BookGenreDecorator(adaptedIt, BookGenre::Science), true);
     ReadAll(adaptedScienceIt);
     delete adaptedScienceIt;
     // adaptedIt удалять не надо, так как он удаляется внутри декоратора adaptedScienceIt


     return 0;
 }
