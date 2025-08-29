#include "lib/data.h"
#include "lib/lib.h"

// data setup
int setup() {
  // =====================================================
  // CREATE AUTHORS
  // =====================================================
  const int tolkien = member_add((member){
      .first_name = s("J.R.R."),
      .last_name = s("Tolkien"),
      .type = AUTHOR,
      .o.author.genre = s("Fantasy"),
  });

  const int rowling = member_add((member){
      .first_name = s("Joanne"),
      .last_name = s("Rowling"),
      .type = AUTHOR,
      .o.author.genre = s("Fantasy"),
  });

  const int king = member_add((member){
      .first_name = s("Stephen"),
      .last_name = s("King"),
      .type = AUTHOR,
      .o.author.genre = s("Horror"),
  });

  const int austen = member_add((member){
      .first_name = s("Jane"),
      .last_name = s("Austen"),
      .type = AUTHOR,
      .o.author.genre = s("Romance"),
  });

  const int orwell = member_add((member){
      .first_name = s("George"),
      .last_name = s("Orwell"),
      .type = AUTHOR,
      .o.author.genre = s("Dystopian"),
  });

  const int hemingway = member_add((member){
      .first_name = s("Ernest"),
      .last_name = s("Hemingway"),
      .type = AUTHOR,
      .o.author.genre = s("Literary Fiction"),
  });

  const int christie = member_add((member){
      .first_name = s("Agatha"),
      .last_name = s("Christie"),
      .type = AUTHOR,
      .o.author.genre = s("Mystery"),
  });

  const int asimov = member_add((member){
      .first_name = s("Isaac"),
      .last_name = s("Asimov"),
      .type = AUTHOR,
      .o.author.genre = s("Science Fiction"),
  });

  const int dickens = member_add((member){
      .first_name = s("Charles"),
      .last_name = s("Dickens"),
      .type = AUTHOR,
      .o.author.genre = s("Historical Fiction"),
  });

  const int adams = member_add((member){
      .first_name = s("Douglas"),
      .last_name = s("Adams"),
      .type = AUTHOR,
      .o.author.genre = s("Science Fiction"),
  });

  // =====================================================
  // CREATE BOOKS
  // =====================================================
  const int the_hobbit = book_add((book){
      .title = "The Hobbit",
      .id_author = tolkien,
      .publication_date =
          (date){
              .year = 1937,
              .month = 9,
              .day = 21,
              .hour = 0,
              .minute = 0,
              .second = 0,
          },
      .ISBN = "978-0261102217",
      .genre = "Fantasy",
      .available = 10,
      .count = 10,
  });

  const int harry_potter = book_add((book){
      .title = "Harry Potter and the Philosopher's Stone",
      .id_author = rowling,
      .publication_date =
          (date){
              .year = 1997,
              .month = 6,
              .day = 26,
              .hour = 0,
              .minute = 0,
              .second = 0,
          },
      .ISBN = "978-0747532699",
      .genre = "Fantasy",
      .available = 10,
      .count = 10,
  });

  const int the_shining = book_add((book){
      .title = "The Shining",
      .id_author = king,
      .publication_date =
          (date){
              .year = 1977,
              .month = 1,
              .day = 28,
              .hour = 0,
              .minute = 0,
              .second = 0,
          },
      .ISBN = "978-0385121675",
      .genre = "Horror",
      .available = 10,
      .count = 10,
  });

  const int pride_and_prejudice = book_add((book){
      .title = "Pride and Prejudice",
      .id_author = austen,
      .publication_date =
          (date){
              .year = 1813,
              .month = 1,
              .day = 28,
              .hour = 0,
              .minute = 0,
              .second = 0,
          },
      .ISBN = "978-0141439518",
      .genre = "Romance",
      .available = 10,
      .count = 10,
  });

  const int nineteen_eighty_four = book_add((book){
      .title = "1984",
      .id_author = orwell,
      .publication_date =
          (date){
              .year = 1949,
              .month = 6,
              .day = 8,
              .hour = 0,
              .minute = 0,
              .second = 0,
          },
      .ISBN = "978-0451524935",
      .genre = "Dystopian",
      .available = 10,
      .count = 10,
  });

  const int old_man_and_sea = book_add((book){
      .title = "The Old Man and the Sea",
      .id_author = hemingway,
      .publication_date =
          (date){
              .year = 1952,
              .month = 9,
              .day = 1,
              .hour = 0,
              .minute = 0,
              .second = 0,
          },
      .ISBN = "978-0684830490",
      .genre = "Literary Fiction",
      .available = 10,
      .count = 10,
  });

  const int poirot_murder = book_add((book){
      .title = "Murder on the Orient Express",
      .id_author = christie,
      .publication_date =
          (date){
              .year = 1934,
              .month = 1,
              .day = 1,
              .hour = 0,
              .minute = 0,
              .second = 0,
          },
      .ISBN = "978-0062693662",
      .genre = "Mystery",
      .available = 10,
      .count = 10,
  });

  const int foundation = book_add((book){
      .title = "Foundation",
      .id_author = asimov,
      .publication_date =
          (date){
              .year = 1951,
              .month = 5,
              .day = 1,
              .hour = 0,
              .minute = 0,
              .second = 0,
          },
      .ISBN = "978-0553293357",
      .genre = "Science Fiction",
      .available = 10,
      .count = 10,
  });

  const int great_expectations = book_add((book){
      .title = "Great Expectations",
      .id_author = dickens,
      .publication_date =
          (date){
              .year = 1861,
              .month = 8,
              .day = 1,
              .hour = 0,
              .minute = 0,
              .second = 0,
          },
      .ISBN = "978-0141439563",
      .genre = "Historical Fiction",
      .available = 10,
      .count = 10,
  });

  const int hitchhiker = book_add((book){
      .title = "The Hitchhiker's Guide to the Galaxy",
      .id_author = adams,
      .publication_date =
          (date){
              .year = 1979,
              .month = 10,
              .day = 12,
              .hour = 0,
              .minute = 0,
              .second = 0,
          },
      .ISBN = "978-0345391803",
      .genre = "Science Fiction",
      .available = 10,
      .count = 10,
  });

  // =====================================================
  // CREATE USERS
  // =====================================================
  const int alice_smith =
      member_add((member){.first_name = s("Alice"),
                          .last_name = s("Smith"),
                          .dob =
                              (date){
                                  .year = 1990,
                                  .month = 3,
                                  .day = 15,
                                  .hour = 0,
                                  .minute = 0,
                                  .second = 0,
                              },
                          .type = MEMBER,
                          .email = s("alice.smith@email.com"),
                          .phone_number = s("555-0101"),
                          .time_created = date_now()});

  const int bob_johnson =
      member_add((member){.first_name = s("Bob"),
                          .last_name = s("Johnson"),
                          .dob =
                              (date){
                                  .year = 1985,
                                  .month = 7,
                                  .day = 22,
                                  .hour = 0,
                                  .minute = 0,
                                  .second = 0,
                              },
                          .type = MEMBER,
                          .email = s("bob.johnson@email.com"),
                          .phone_number = s("555-0102"),
                          .time_created = date_now()});

  const int clara_wong = member_add((member){.first_name = s("Clara"),
                                             .last_name = s("Wong"),
                                             .dob =
                                                 (date){
                                                     .year = 1995,
                                                     .month = 11,
                                                     .day = 8,
                                                     .hour = 0,
                                                     .minute = 0,
                                                     .second = 0,
                                                 },
                                             .type = MEMBER,
                                             .email = s("clara.wong@email.com"),
                                             .phone_number = s("555-0103"),
                                             .time_created = date_now()});

  const int david_martinez =
      member_add((member){.first_name = s("David"),
                          .last_name = s("Martinez"),
                          .dob =
                              (date){
                                  .year = 1988,
                                  .month = 4,
                                  .day = 30,
                                  .hour = 0,
                                  .minute = 0,
                                  .second = 0,
                              },
                          .type = MEMBER,
                          .email = s("david.martinez@email.com"),
                          .phone_number = s("555-0104"),
                          .time_created = date_now()});

  const int emma_brown = member_add((member){.first_name = s("Emma"),
                                             .last_name = s("Brown"),
                                             .dob =
                                                 (date){
                                                     .year = 1992,
                                                     .month = 9,
                                                     .day = 12,
                                                     .hour = 0,
                                                     .minute = 0,
                                                     .second = 0,
                                                 },
                                             .type = MEMBER,
                                             .email = s("emma.brown@email.com"),
                                             .phone_number = s("555-0105"),
                                             .time_created = date_now()});

  const int frank_lee = member_add((member){.first_name = s("Frank"),
                                            .last_name = s("Lee"),
                                            .dob =
                                                (date){
                                                    .year = 1983,
                                                    .month = 2,
                                                    .day = 19,
                                                    .hour = 0,
                                                    .minute = 0,
                                                    .second = 0,
                                                },
                                            .type = MEMBER,
                                            .email = s("frank.lee@email.com"),
                                            .phone_number = s("555-0106"),
                                            .time_created = date_now()});

  const int grace_kim = member_add((member){.first_name = s("Grace"),
                                            .last_name = s("Kim"),
                                            .dob =
                                                (date){
                                                    .year = 1998,
                                                    .month = 12,
                                                    .day = 5,
                                                    .hour = 0,
                                                    .minute = 0,
                                                    .second = 0,
                                                },
                                            .type = MEMBER,
                                            .email = s("grace.kim@email.com"),
                                            .phone_number = s("555-0107"),
                                            .time_created = date_now()});

  const int henry_davis =
      member_add((member){.first_name = s("Henry"),
                          .last_name = s("Davis"),
                          .dob =
                              (date){
                                  .year = 1980,
                                  .month = 6,
                                  .day = 25,
                                  .hour = 0,
                                  .minute = 0,
                                  .second = 0,
                              },
                          .type = MEMBER,
                          .email = s("henry.davis@email.com"),
                          .phone_number = s("555-0108"),
                          .time_created = date_now()});

  const int isabella_nguyen =
      member_add((member){.first_name = s("Isabella"),
                          .last_name = s("Nguyen"),
                          .dob =
                              (date){
                                  .year = 1993,
                                  .month = 8,
                                  .day = 17,
                                  .hour = 0,
                                  .minute = 0,
                                  .second = 0,
                              },
                          .type = MEMBER,
                          .email = s("isabella.nguyen@email.com"),
                          .phone_number = s("555-0109"),
                          .time_created = date_now()});

  const int james_park = member_add((member){.first_name = s("James"),
                                             .last_name = s("Park"),
                                             .dob =
                                                 (date){
                                                     .year = 1987,
                                                     .month = 1,
                                                     .day = 10,
                                                     .hour = 0,
                                                     .minute = 0,
                                                     .second = 0,
                                                 },
                                             .type = MEMBER,
                                             .email = s("james.park@email.com"),
                                             .phone_number = s("555-0110"),
                                             .time_created = date_now()});

  return 0;

  // ======================================================
  // CREATE STAFF
  // ======================================================
  const int member_1 = member_add((member){.type = STAFF,
                                           .first_name = s("James"),
                                           .last_name = s("Park"),
                                           .dob =
                                               (date){
                                                   .year = 1987,
                                                   .month = 1,
                                                   .day = 10,
                                                   .hour = 0,
                                                   .minute = 0,
                                                   .second = 0,
                                               },
                                           .email = s("james.park@email.com"),
                                           .phone_number = s("555-0110"),
                                           .time_created = date_now(),
                                           .o.staff = {
                                               .member_code = db_members_index,
                                               .member_id = db_members_index,
                                               .is_hired = true,
                                           }});

  const int member_2 = member_add((member){.type = STAFF,
                                           .first_name = s("Emma"),
                                           .last_name = s("Wilson"),
                                           .dob =
                                               (date){
                                                   .year = 1990,
                                                   .month = 5,
                                                   .day = 22,
                                                   .hour = 0,
                                                   .minute = 0,
                                                   .second = 0,
                                               },
                                           .email = s("emma.wilson@email.com"),
                                           .phone_number = s("555-0111"),
                                           .time_created = date_now(),
                                           .o.staff = {
                                               .member_code = db_members_index,
                                               .member_id = db_members_index,
                                               .is_hired = true,
                                           }});

  const int member_3 = member_add((member){.type = STAFF,
                                           .first_name = s("Liam"),
                                           .last_name = s("Chen"),
                                           .dob =
                                               (date){
                                                   .year = 1985,
                                                   .month = 8,
                                                   .day = 15,
                                                   .hour = 0,
                                                   .minute = 0,
                                                   .second = 0,
                                               },
                                           .email = s("liam.chen@email.com"),
                                           .phone_number = s("555-0112"),
                                           .time_created = date_now(),
                                           .o.staff = {
                                               .member_code = db_members_index,
                                               .member_id = db_members_index,
                                               .is_hired = true,
                                           }});

  const int member_4 =
      member_add((member){.type = STAFF,
                          .first_name = s("Sophia"),
                          .last_name = s("Martinez"),
                          .dob =
                              (date){
                                  .year = 1992,
                                  .month = 3,
                                  .day = 7,
                                  .hour = 0,
                                  .minute = 0,
                                  .second = 0,
                              },
                          .email = s("sophia.martinez@email.com"),
                          .phone_number = s("555-0113"),
                          .time_created = date_now(),
                          .o.staff = {
                              .member_code = db_members_index,
                              .member_id = db_members_index,
                              .is_hired = true,
                          }});

  const int member_5 = member_add((member){.type = STAFF,
                                           .first_name = s("Noah"),
                                           .last_name = s("Davis"),
                                           .dob =
                                               (date){
                                                   .year = 1988,
                                                   .month = 11,
                                                   .day = 30,
                                                   .hour = 0,
                                                   .minute = 0,
                                                   .second = 0,
                                               },
                                           .email = s("noah.davis@email.com"),
                                           .phone_number = s("555-0114"),
                                           .time_created = date_now(),
                                           .o.staff = {
                                               .member_code = db_members_index,
                                               .member_id = db_members_index,
                                               .is_hired = true,
                                           }});

  const int member_6 = member_add((member){.type = STAFF,
                                           .first_name = s("Olivia"),
                                           .last_name = s("Brown"),
                                           .dob =
                                               (date){
                                                   .year = 1995,
                                                   .month = 7,
                                                   .day = 19,
                                                   .hour = 0,
                                                   .minute = 0,
                                                   .second = 0,
                                               },
                                           .email = s("olivia.brown@email.com"),
                                           .phone_number = s("555-0115"),
                                           .time_created = date_now(),
                                           .o.staff = {
                                               .member_code = db_members_index,
                                               .member_id = db_members_index,
                                               .is_hired = true,
                                           }});

  const int member_7 = member_add((member){.type = STAFF,
                                           .first_name = s("Ethan"),
                                           .last_name = s("Taylor"),
                                           .dob =
                                               (date){
                                                   .year = 1983,
                                                   .month = 2,
                                                   .day = 25,
                                                   .hour = 0,
                                                   .minute = 0,
                                                   .second = 0,
                                               },
                                           .email = s("ethan.taylor@email.com"),
                                           .phone_number = s("555-0116"),
                                           .time_created = date_now(),
                                           .o.staff = {
                                               .member_code = db_members_index,
                                               .member_id = db_members_index,
                                               .is_hired = true,
                                           }});

  const int member_8 = member_add((member){.type = STAFF,
                                           .first_name = s("Ava"),
                                           .last_name = s("Lee"),
                                           .dob =
                                               (date){
                                                   .year = 1991,
                                                   .month = 9,
                                                   .day = 12,
                                                   .hour = 0,
                                                   .minute = 0,
                                                   .second = 0,
                                               },
                                           .email = s("ava.lee@email.com"),
                                           .phone_number = s("555-0117"),
                                           .time_created = date_now(),
                                           .o.staff = {
                                               .member_code = db_members_index,
                                               .member_id = db_members_index,
                                               .is_hired = true,
                                           }});

  const int member_9 = member_add((member){.type = STAFF,
                                           .first_name = s("Lucas"),
                                           .last_name = s("Garcia"),
                                           .dob =
                                               (date){
                                                   .year = 1989,
                                                   .month = 4,
                                                   .day = 5,
                                                   .hour = 0,
                                                   .minute = 0,
                                                   .second = 0,
                                               },
                                           .email = s("lucas.garcia@email.com"),
                                           .phone_number = s("555-0118"),
                                           .time_created = date_now(),
                                           .o.staff = {
                                               .member_code = db_members_index,
                                               .member_id = db_members_index,
                                               .is_hired = true,
                                           }});

  const int member_10 = member_add((member){.type = STAFF,
                                            .first_name = s("Mia"),
                                            .last_name = s("Johnson"),
                                            .dob =
                                                (date){
                                                    .year = 1994,
                                                    .month = 6,
                                                    .day = 28,
                                                    .hour = 0,
                                                    .minute = 0,
                                                    .second = 0,
                                                },
                                            .email = s("mia.johnson@email.com"),
                                            .phone_number = s("555-0119"),
                                            .time_created = date_now(),
                                            .o.staff = {
                                                .member_code = db_members_index,
                                                .member_id = db_members_index,
                                                .is_hired = true,
                                            }});
}

// database code
int database() { return 12; }
