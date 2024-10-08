PGDMP     6        	            z            Plain    10.18    10.18 �    �           0    0    ENCODING    ENCODING        SET client_encoding = 'UTF8';
                       false            �           0    0 
   STDSTRINGS 
   STDSTRINGS     (   SET standard_conforming_strings = 'on';
                       false            �           0    0 
   SEARCHPATH 
   SEARCHPATH     8   SELECT pg_catalog.set_config('search_path', '', false);
                       false            �           1262    16393    Plain    DATABASE     �   CREATE DATABASE "Plain" WITH TEMPLATE = template0 ENCODING = 'UTF8' LC_COLLATE = 'English_United States.1252' LC_CTYPE = 'English_United States.1252';
    DROP DATABASE "Plain";
             postgres    false            �           0    0    DATABASE "Plain"    COMMENT     k   COMMENT ON DATABASE "Plain" IS 'Plain is a video game digital distribution service and blah blah blah...';
                  postgres    false    2991                        2615    2200    public    SCHEMA        CREATE SCHEMA public;
    DROP SCHEMA public;
             postgres    false            �           0    0    SCHEMA public    COMMENT     6   COMMENT ON SCHEMA public IS 'standard public schema';
                  postgres    false    3                        3079    12924    plpgsql 	   EXTENSION     ?   CREATE EXTENSION IF NOT EXISTS plpgsql WITH SCHEMA pg_catalog;
    DROP EXTENSION plpgsql;
                  false            �           0    0    EXTENSION plpgsql    COMMENT     @   COMMENT ON EXTENSION plpgsql IS 'PL/pgSQL procedural language';
                       false    1            �            1259    16533    achievement    TABLE     �   CREATE TABLE public.achievement (
    id bigint NOT NULL,
    g_id integer NOT NULL,
    a_name character varying(64) NOT NULL,
    a_description character varying(256) NOT NULL
);
    DROP TABLE public.achievement;
       public         postgres    false    3            �            1259    16531    a_id_seq    SEQUENCE     p   CREATE SEQUENCE public.a_id_seq
    START WITH 0
    INCREMENT BY 1
    MINVALUE 0
    NO MAXVALUE
    CACHE 1;
    DROP SEQUENCE public.a_id_seq;
       public       postgres    false    209    3            �           0    0    a_id_seq    SEQUENCE OWNED BY     ?   ALTER SEQUENCE public.a_id_seq OWNED BY public.achievement.id;
            public       postgres    false    208            �            1259    16744    cart_inventory    TABLE     �   CREATE TABLE public.cart_inventory (
    id bigint NOT NULL,
    i_id integer NOT NULL,
    u_id integer NOT NULL,
    amount integer NOT NULL
);
 "   DROP TABLE public.cart_inventory;
       public         postgres    false    3            �            1259    16742 
   c_i_id_seq    SEQUENCE     s   CREATE SEQUENCE public.c_i_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 !   DROP SEQUENCE public.c_i_id_seq;
       public       postgres    false    225    3            �           0    0 
   c_i_id_seq    SEQUENCE OWNED BY     D   ALTER SEQUENCE public.c_i_id_seq OWNED BY public.cart_inventory.id;
            public       postgres    false    224            �            1259    16726    cart    TABLE     k   CREATE TABLE public.cart (
    id bigint NOT NULL,
    g_id integer NOT NULL,
    u_id integer NOT NULL
);
    DROP TABLE public.cart;
       public         postgres    false    3            �            1259    16724    c_id_seq    SEQUENCE     q   CREATE SEQUENCE public.c_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
    DROP SEQUENCE public.c_id_seq;
       public       postgres    false    3    223            �           0    0    c_id_seq    SEQUENCE OWNED BY     8   ALTER SEQUENCE public.c_id_seq OWNED BY public.cart.id;
            public       postgres    false    222            �            1259    16467 	   developer    TABLE     g   CREATE TABLE public.developer (
    id integer NOT NULL,
    d_name character varying(128) NOT NULL
);
    DROP TABLE public.developer;
       public         postgres    false    3            �            1259    16465    d_id_seq    SEQUENCE     �   CREATE SEQUENCE public.d_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
    DROP SEQUENCE public.d_id_seq;
       public       postgres    false    3    201            �           0    0    d_id_seq    SEQUENCE OWNED BY     =   ALTER SEQUENCE public.d_id_seq OWNED BY public.developer.id;
            public       postgres    false    200            �            1259    16487    game_developer    TABLE     v   CREATE TABLE public.game_developer (
    id integer NOT NULL,
    g_id integer NOT NULL,
    d_id integer NOT NULL
);
 "   DROP TABLE public.game_developer;
       public         postgres    false    3            �            1259    16485 
   g_d_id_seq    SEQUENCE     �   CREATE SEQUENCE public.g_d_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 !   DROP SEQUENCE public.g_d_id_seq;
       public       postgres    false    3    205            �           0    0 
   g_d_id_seq    SEQUENCE OWNED BY     D   ALTER SEQUENCE public.g_d_id_seq OWNED BY public.game_developer.id;
            public       postgres    false    204            �            1259    16456    game    TABLE     �   CREATE TABLE public.game (
    id integer NOT NULL,
    g_name character varying(128) NOT NULL,
    g_price double precision NOT NULL,
    g_sdescription character varying(1024),
    g_description character varying(8192)
);
    DROP TABLE public.game;
       public         postgres    false    3            �            1259    16454    g_id_seq    SEQUENCE        CREATE SEQUENCE public.g_id_seq
    AS integer
    START WITH 0
    INCREMENT BY 1
    MINVALUE 0
    NO MAXVALUE
    CACHE 1;
    DROP SEQUENCE public.g_id_seq;
       public       postgres    false    3    199            �           0    0    g_id_seq    SEQUENCE OWNED BY     8   ALTER SEQUENCE public.g_id_seq OWNED BY public.game.id;
            public       postgres    false    198            �            1259    16508    game_publisher    TABLE     v   CREATE TABLE public.game_publisher (
    id integer NOT NULL,
    g_id integer NOT NULL,
    p_id integer NOT NULL
);
 "   DROP TABLE public.game_publisher;
       public         postgres    false    3            �            1259    16506 
   g_p_id_seq    SEQUENCE     �   CREATE SEQUENCE public.g_p_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 !   DROP SEQUENCE public.g_p_id_seq;
       public       postgres    false    3    207            �           0    0 
   g_p_id_seq    SEQUENCE OWNED BY     D   ALTER SEQUENCE public.g_p_id_seq OWNED BY public.game_publisher.id;
            public       postgres    false    206            �            1259    16568 	   inventory    TABLE     �   CREATE TABLE public.inventory (
    id bigint NOT NULL,
    g_id integer NOT NULL,
    i_name character varying(64) NOT NULL,
    i_price double precision NOT NULL,
    i_description character varying(1024) NOT NULL
);
    DROP TABLE public.inventory;
       public         postgres    false    3            �            1259    16566    i_id_seq    SEQUENCE     q   CREATE SEQUENCE public.i_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
    DROP SEQUENCE public.i_id_seq;
       public       postgres    false    3    211            �           0    0    i_id_seq    SEQUENCE OWNED BY     =   ALTER SEQUENCE public.i_id_seq OWNED BY public.inventory.id;
            public       postgres    false    210            �            1259    16690    library    TABLE     �   CREATE TABLE public.library (
    id bigint NOT NULL,
    g_id integer NOT NULL,
    u_id integer NOT NULL,
    l_timeplayed integer NOT NULL,
    l_installed boolean NOT NULL
);
    DROP TABLE public.library;
       public         postgres    false    3            �            1259    16688    l_id_seq    SEQUENCE     q   CREATE SEQUENCE public.l_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
    DROP SEQUENCE public.l_id_seq;
       public       postgres    false    219    3            �           0    0    l_id_seq    SEQUENCE OWNED BY     ;   ALTER SEQUENCE public.l_id_seq OWNED BY public.library.id;
            public       postgres    false    218            �            1259    16477 	   publisher    TABLE     g   CREATE TABLE public.publisher (
    id integer NOT NULL,
    p_name character varying(128) NOT NULL
);
    DROP TABLE public.publisher;
       public         postgres    false    3            �            1259    16475    p_id_seq    SEQUENCE     �   CREATE SEQUENCE public.p_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
    DROP SEQUENCE public.p_id_seq;
       public       postgres    false    3    203            �           0    0    p_id_seq    SEQUENCE OWNED BY     =   ALTER SEQUENCE public.p_id_seq OWNED BY public.publisher.id;
            public       postgres    false    202            �            1259    16656    review    TABLE     �   CREATE TABLE public.review (
    id bigint NOT NULL,
    g_id integer NOT NULL,
    r_text character varying(1024),
    r_rating integer NOT NULL
);
    DROP TABLE public.review;
       public         postgres    false    3            �            1259    16654    r_id_seq    SEQUENCE     q   CREATE SEQUENCE public.r_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
    DROP SEQUENCE public.r_id_seq;
       public       postgres    false    215    3            �           0    0    r_id_seq    SEQUENCE OWNED BY     :   ALTER SEQUENCE public.r_id_seq OWNED BY public.review.id;
            public       postgres    false    214            �            1259    16638    user_achievement    TABLE     w   CREATE TABLE public.user_achievement (
    id bigint NOT NULL,
    u_id integer NOT NULL,
    a_id integer NOT NULL
);
 $   DROP TABLE public.user_achievement;
       public         postgres    false    3            �            1259    16636 
   u_a_id_seq    SEQUENCE     s   CREATE SEQUENCE public.u_a_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 !   DROP SEQUENCE public.u_a_id_seq;
       public       postgres    false    3    213            �           0    0 
   u_a_id_seq    SEQUENCE OWNED BY     F   ALTER SEQUENCE public.u_a_id_seq OWNED BY public.user_achievement.id;
            public       postgres    false    212            �            1259    16708    user_inventory    TABLE     �   CREATE TABLE public.user_inventory (
    id bigint NOT NULL,
    u_id integer NOT NULL,
    i_id integer NOT NULL,
    u_i_amount integer NOT NULL
);
 "   DROP TABLE public.user_inventory;
       public         postgres    false    3            �            1259    16706 
   u_i_id_seq    SEQUENCE     s   CREATE SEQUENCE public.u_i_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 !   DROP SEQUENCE public.u_i_id_seq;
       public       postgres    false    221    3            �           0    0 
   u_i_id_seq    SEQUENCE OWNED BY     D   ALTER SEQUENCE public.u_i_id_seq OWNED BY public.user_inventory.id;
            public       postgres    false    220            �            1259    16422    u_id_seq    SEQUENCE     p   CREATE SEQUENCE public.u_id_seq
    START WITH 0
    INCREMENT BY 1
    MINVALUE 0
    NO MAXVALUE
    CACHE 1;
    DROP SEQUENCE public.u_id_seq;
       public       postgres    false    3            �            1259    16672    user_review    TABLE     r   CREATE TABLE public.user_review (
    id bigint NOT NULL,
    u_id integer NOT NULL,
    r_id integer NOT NULL
);
    DROP TABLE public.user_review;
       public         postgres    false    3            �            1259    16670 
   u_r_id_seq    SEQUENCE     s   CREATE SEQUENCE public.u_r_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 !   DROP SEQUENCE public.u_r_id_seq;
       public       postgres    false    217    3            �           0    0 
   u_r_id_seq    SEQUENCE OWNED BY     A   ALTER SEQUENCE public.u_r_id_seq OWNED BY public.user_review.id;
            public       postgres    false    216            �            1259    16395    user    TABLE       CREATE TABLE public."user" (
    id integer DEFAULT nextval('public.u_id_seq'::regclass) NOT NULL,
    u_login character varying(16) NOT NULL,
    u_password character varying(16) NOT NULL,
    u_username character varying(16) NOT NULL,
    u_admin character varying(16)
);
    DROP TABLE public."user";
       public         postgres    false    197    3            �
           2604    16536    achievement id    DEFAULT     f   ALTER TABLE ONLY public.achievement ALTER COLUMN id SET DEFAULT nextval('public.a_id_seq'::regclass);
 =   ALTER TABLE public.achievement ALTER COLUMN id DROP DEFAULT;
       public       postgres    false    209    208    209            �
           2604    16729    cart id    DEFAULT     _   ALTER TABLE ONLY public.cart ALTER COLUMN id SET DEFAULT nextval('public.c_id_seq'::regclass);
 6   ALTER TABLE public.cart ALTER COLUMN id DROP DEFAULT;
       public       postgres    false    223    222    223            �
           2604    16747    cart_inventory id    DEFAULT     k   ALTER TABLE ONLY public.cart_inventory ALTER COLUMN id SET DEFAULT nextval('public.c_i_id_seq'::regclass);
 @   ALTER TABLE public.cart_inventory ALTER COLUMN id DROP DEFAULT;
       public       postgres    false    224    225    225            �
           2604    16470    developer id    DEFAULT     d   ALTER TABLE ONLY public.developer ALTER COLUMN id SET DEFAULT nextval('public.d_id_seq'::regclass);
 ;   ALTER TABLE public.developer ALTER COLUMN id DROP DEFAULT;
       public       postgres    false    200    201    201            �
           2604    16619    game id    DEFAULT     _   ALTER TABLE ONLY public.game ALTER COLUMN id SET DEFAULT nextval('public.g_id_seq'::regclass);
 6   ALTER TABLE public.game ALTER COLUMN id DROP DEFAULT;
       public       postgres    false    199    198    199            �
           2604    16490    game_developer id    DEFAULT     k   ALTER TABLE ONLY public.game_developer ALTER COLUMN id SET DEFAULT nextval('public.g_d_id_seq'::regclass);
 @   ALTER TABLE public.game_developer ALTER COLUMN id DROP DEFAULT;
       public       postgres    false    204    205    205            �
           2604    16511    game_publisher id    DEFAULT     k   ALTER TABLE ONLY public.game_publisher ALTER COLUMN id SET DEFAULT nextval('public.g_p_id_seq'::regclass);
 @   ALTER TABLE public.game_publisher ALTER COLUMN id DROP DEFAULT;
       public       postgres    false    207    206    207            �
           2604    16571    inventory id    DEFAULT     d   ALTER TABLE ONLY public.inventory ALTER COLUMN id SET DEFAULT nextval('public.i_id_seq'::regclass);
 ;   ALTER TABLE public.inventory ALTER COLUMN id DROP DEFAULT;
       public       postgres    false    210    211    211            �
           2604    16693 
   library id    DEFAULT     b   ALTER TABLE ONLY public.library ALTER COLUMN id SET DEFAULT nextval('public.l_id_seq'::regclass);
 9   ALTER TABLE public.library ALTER COLUMN id DROP DEFAULT;
       public       postgres    false    219    218    219            �
           2604    16480    publisher id    DEFAULT     d   ALTER TABLE ONLY public.publisher ALTER COLUMN id SET DEFAULT nextval('public.p_id_seq'::regclass);
 ;   ALTER TABLE public.publisher ALTER COLUMN id DROP DEFAULT;
       public       postgres    false    203    202    203            �
           2604    16659 	   review id    DEFAULT     a   ALTER TABLE ONLY public.review ALTER COLUMN id SET DEFAULT nextval('public.r_id_seq'::regclass);
 8   ALTER TABLE public.review ALTER COLUMN id DROP DEFAULT;
       public       postgres    false    215    214    215            �
           2604    16641    user_achievement id    DEFAULT     m   ALTER TABLE ONLY public.user_achievement ALTER COLUMN id SET DEFAULT nextval('public.u_a_id_seq'::regclass);
 B   ALTER TABLE public.user_achievement ALTER COLUMN id DROP DEFAULT;
       public       postgres    false    213    212    213            �
           2604    16711    user_inventory id    DEFAULT     k   ALTER TABLE ONLY public.user_inventory ALTER COLUMN id SET DEFAULT nextval('public.u_i_id_seq'::regclass);
 @   ALTER TABLE public.user_inventory ALTER COLUMN id DROP DEFAULT;
       public       postgres    false    220    221    221            �
           2604    16675    user_review id    DEFAULT     h   ALTER TABLE ONLY public.user_review ALTER COLUMN id SET DEFAULT nextval('public.u_r_id_seq'::regclass);
 =   ALTER TABLE public.user_review ALTER COLUMN id DROP DEFAULT;
       public       postgres    false    216    217    217            �          0    16533    achievement 
   TABLE DATA               F   COPY public.achievement (id, g_id, a_name, a_description) FROM stdin;
    public       postgres    false    209   ��       �          0    16726    cart 
   TABLE DATA               .   COPY public.cart (id, g_id, u_id) FROM stdin;
    public       postgres    false    223   |�       �          0    16744    cart_inventory 
   TABLE DATA               @   COPY public.cart_inventory (id, i_id, u_id, amount) FROM stdin;
    public       postgres    false    225   ��       �          0    16467 	   developer 
   TABLE DATA               /   COPY public.developer (id, d_name) FROM stdin;
    public       postgres    false    201   ��       �          0    16456    game 
   TABLE DATA               R   COPY public.game (id, g_name, g_price, g_sdescription, g_description) FROM stdin;
    public       postgres    false    199   ��       �          0    16487    game_developer 
   TABLE DATA               8   COPY public.game_developer (id, g_id, d_id) FROM stdin;
    public       postgres    false    205   ��       �          0    16508    game_publisher 
   TABLE DATA               8   COPY public.game_publisher (id, g_id, p_id) FROM stdin;
    public       postgres    false    207   R�       �          0    16568 	   inventory 
   TABLE DATA               M   COPY public.inventory (id, g_id, i_name, i_price, i_description) FROM stdin;
    public       postgres    false    211   Ҟ       �          0    16690    library 
   TABLE DATA               L   COPY public.library (id, g_id, u_id, l_timeplayed, l_installed) FROM stdin;
    public       postgres    false    219   $�       �          0    16477 	   publisher 
   TABLE DATA               /   COPY public.publisher (id, p_name) FROM stdin;
    public       postgres    false    203   G�       �          0    16656    review 
   TABLE DATA               <   COPY public.review (id, g_id, r_text, r_rating) FROM stdin;
    public       postgres    false    215   �       �          0    16395    user 
   TABLE DATA               N   COPY public."user" (id, u_login, u_password, u_username, u_admin) FROM stdin;
    public       postgres    false    196   3�       �          0    16638    user_achievement 
   TABLE DATA               :   COPY public.user_achievement (id, u_id, a_id) FROM stdin;
    public       postgres    false    213   ��       �          0    16708    user_inventory 
   TABLE DATA               D   COPY public.user_inventory (id, u_id, i_id, u_i_amount) FROM stdin;
    public       postgres    false    221   
�       �          0    16672    user_review 
   TABLE DATA               5   COPY public.user_review (id, u_id, r_id) FROM stdin;
    public       postgres    false    217   '�       �           0    0    a_id_seq    SEQUENCE SET     7   SELECT pg_catalog.setval('public.a_id_seq', 28, true);
            public       postgres    false    208            �           0    0 
   c_i_id_seq    SEQUENCE SET     9   SELECT pg_catalog.setval('public.c_i_id_seq', 1, false);
            public       postgres    false    224            �           0    0    c_id_seq    SEQUENCE SET     7   SELECT pg_catalog.setval('public.c_id_seq', 1, false);
            public       postgres    false    222            �           0    0    d_id_seq    SEQUENCE SET     7   SELECT pg_catalog.setval('public.d_id_seq', 15, true);
            public       postgres    false    200            �           0    0 
   g_d_id_seq    SEQUENCE SET     9   SELECT pg_catalog.setval('public.g_d_id_seq', 38, true);
            public       postgres    false    204            �           0    0    g_id_seq    SEQUENCE SET     7   SELECT pg_catalog.setval('public.g_id_seq', 29, true);
            public       postgres    false    198            �           0    0 
   g_p_id_seq    SEQUENCE SET     9   SELECT pg_catalog.setval('public.g_p_id_seq', 37, true);
            public       postgres    false    206            �           0    0    i_id_seq    SEQUENCE SET     6   SELECT pg_catalog.setval('public.i_id_seq', 4, true);
            public       postgres    false    210            �           0    0    l_id_seq    SEQUENCE SET     7   SELECT pg_catalog.setval('public.l_id_seq', 1, false);
            public       postgres    false    218            �           0    0    p_id_seq    SEQUENCE SET     7   SELECT pg_catalog.setval('public.p_id_seq', 13, true);
            public       postgres    false    202            �           0    0    r_id_seq    SEQUENCE SET     7   SELECT pg_catalog.setval('public.r_id_seq', 1, false);
            public       postgres    false    214            �           0    0 
   u_a_id_seq    SEQUENCE SET     9   SELECT pg_catalog.setval('public.u_a_id_seq', 1, false);
            public       postgres    false    212            �           0    0 
   u_i_id_seq    SEQUENCE SET     9   SELECT pg_catalog.setval('public.u_i_id_seq', 1, false);
            public       postgres    false    220            �           0    0    u_id_seq    SEQUENCE SET     7   SELECT pg_catalog.setval('public.u_id_seq', 46, true);
            public       postgres    false    197            �           0    0 
   u_r_id_seq    SEQUENCE SET     9   SELECT pg_catalog.setval('public.u_r_id_seq', 1, false);
            public       postgres    false    216            �
           2606    16664    review Review_pkey 
   CONSTRAINT     R   ALTER TABLE ONLY public.review
    ADD CONSTRAINT "Review_pkey" PRIMARY KEY (id);
 >   ALTER TABLE ONLY public.review DROP CONSTRAINT "Review_pkey";
       public         postgres    false    215            �
           2606    16538    achievement achievement_pkey 
   CONSTRAINT     Z   ALTER TABLE ONLY public.achievement
    ADD CONSTRAINT achievement_pkey PRIMARY KEY (id);
 F   ALTER TABLE ONLY public.achievement DROP CONSTRAINT achievement_pkey;
       public         postgres    false    209            �
           2606    16749 "   cart_inventory cart_inventory_pkey 
   CONSTRAINT     `   ALTER TABLE ONLY public.cart_inventory
    ADD CONSTRAINT cart_inventory_pkey PRIMARY KEY (id);
 L   ALTER TABLE ONLY public.cart_inventory DROP CONSTRAINT cart_inventory_pkey;
       public         postgres    false    225            �
           2606    16731    cart cart_pkey 
   CONSTRAINT     L   ALTER TABLE ONLY public.cart
    ADD CONSTRAINT cart_pkey PRIMARY KEY (id);
 8   ALTER TABLE ONLY public.cart DROP CONSTRAINT cart_pkey;
       public         postgres    false    223            �
           2606    16474    developer developer_name 
   CONSTRAINT     U   ALTER TABLE ONLY public.developer
    ADD CONSTRAINT developer_name UNIQUE (d_name);
 B   ALTER TABLE ONLY public.developer DROP CONSTRAINT developer_name;
       public         postgres    false    201            �
           2606    16472    developer developer_pkey 
   CONSTRAINT     V   ALTER TABLE ONLY public.developer
    ADD CONSTRAINT developer_pkey PRIMARY KEY (id);
 B   ALTER TABLE ONLY public.developer DROP CONSTRAINT developer_pkey;
       public         postgres    false    201            �
           2606    16492 "   game_developer game_developer_pkey 
   CONSTRAINT     `   ALTER TABLE ONLY public.game_developer
    ADD CONSTRAINT game_developer_pkey PRIMARY KEY (id);
 L   ALTER TABLE ONLY public.game_developer DROP CONSTRAINT game_developer_pkey;
       public         postgres    false    205            �
           2606    16464    game game_name 
   CONSTRAINT     K   ALTER TABLE ONLY public.game
    ADD CONSTRAINT game_name UNIQUE (g_name);
 8   ALTER TABLE ONLY public.game DROP CONSTRAINT game_name;
       public         postgres    false    199            �
           2606    16461    game game_pkey 
   CONSTRAINT     L   ALTER TABLE ONLY public.game
    ADD CONSTRAINT game_pkey PRIMARY KEY (id);
 8   ALTER TABLE ONLY public.game DROP CONSTRAINT game_pkey;
       public         postgres    false    199            �
           2606    16513 "   game_publisher game_publisher_pkey 
   CONSTRAINT     `   ALTER TABLE ONLY public.game_publisher
    ADD CONSTRAINT game_publisher_pkey PRIMARY KEY (id);
 L   ALTER TABLE ONLY public.game_publisher DROP CONSTRAINT game_publisher_pkey;
       public         postgres    false    207            �
           2606    16576    inventory inventory_pkey 
   CONSTRAINT     V   ALTER TABLE ONLY public.inventory
    ADD CONSTRAINT inventory_pkey PRIMARY KEY (id);
 B   ALTER TABLE ONLY public.inventory DROP CONSTRAINT inventory_pkey;
       public         postgres    false    211            �
           2606    16695    library library_pkey 
   CONSTRAINT     R   ALTER TABLE ONLY public.library
    ADD CONSTRAINT library_pkey PRIMARY KEY (id);
 >   ALTER TABLE ONLY public.library DROP CONSTRAINT library_pkey;
       public         postgres    false    219            �
           2606    16484    publisher publisher_name 
   CONSTRAINT     U   ALTER TABLE ONLY public.publisher
    ADD CONSTRAINT publisher_name UNIQUE (p_name);
 B   ALTER TABLE ONLY public.publisher DROP CONSTRAINT publisher_name;
       public         postgres    false    203            �
           2606    16482    publisher publisher_pkey 
   CONSTRAINT     V   ALTER TABLE ONLY public.publisher
    ADD CONSTRAINT publisher_pkey PRIMARY KEY (id);
 B   ALTER TABLE ONLY public.publisher DROP CONSTRAINT publisher_pkey;
       public         postgres    false    203            �
           2606    16643 &   user_achievement user_achievement_pkey 
   CONSTRAINT     d   ALTER TABLE ONLY public.user_achievement
    ADD CONSTRAINT user_achievement_pkey PRIMARY KEY (id);
 P   ALTER TABLE ONLY public.user_achievement DROP CONSTRAINT user_achievement_pkey;
       public         postgres    false    213            �
           2606    16713 "   user_inventory user_inventory_pkey 
   CONSTRAINT     `   ALTER TABLE ONLY public.user_inventory
    ADD CONSTRAINT user_inventory_pkey PRIMARY KEY (id);
 L   ALTER TABLE ONLY public.user_inventory DROP CONSTRAINT user_inventory_pkey;
       public         postgres    false    221            �
           2606    16403    user user_login 
   CONSTRAINT     O   ALTER TABLE ONLY public."user"
    ADD CONSTRAINT user_login UNIQUE (u_login);
 ;   ALTER TABLE ONLY public."user" DROP CONSTRAINT user_login;
       public         postgres    false    196            �
           2606    16399    user user_pkey 
   CONSTRAINT     N   ALTER TABLE ONLY public."user"
    ADD CONSTRAINT user_pkey PRIMARY KEY (id);
 :   ALTER TABLE ONLY public."user" DROP CONSTRAINT user_pkey;
       public         postgres    false    196            �
           2606    16677    user_review user_review_pkey 
   CONSTRAINT     Z   ALTER TABLE ONLY public.user_review
    ADD CONSTRAINT user_review_pkey PRIMARY KEY (id);
 F   ALTER TABLE ONLY public.user_review DROP CONSTRAINT user_review_pkey;
       public         postgres    false    217            �
           1259    16564    fki_ag_id_fkey    INDEX     F   CREATE INDEX fki_ag_id_fkey ON public.achievement USING btree (g_id);
 "   DROP INDEX public.fki_ag_id_fkey;
       public         postgres    false    209            �
           1259    16505    fki_d_id_fkey    INDEX     H   CREATE INDEX fki_d_id_fkey ON public.game_developer USING btree (d_id);
 !   DROP INDEX public.fki_d_id_fkey;
       public         postgres    false    205            �
           1259    16499    fki_g_id_fkey    INDEX     H   CREATE INDEX fki_g_id_fkey ON public.game_developer USING btree (g_id);
 !   DROP INDEX public.fki_g_id_fkey;
       public         postgres    false    205            �
           1259    16524    fki_g_p_game_id_fkey    INDEX     O   CREATE INDEX fki_g_p_game_id_fkey ON public.game_publisher USING btree (g_id);
 (   DROP INDEX public.fki_g_p_game_id_fkey;
       public         postgres    false    207            �
           1259    16582    fki_ig_id_fkey    INDEX     D   CREATE INDEX fki_ig_id_fkey ON public.inventory USING btree (g_id);
 "   DROP INDEX public.fki_ig_id_fkey;
       public         postgres    false    211            �
           1259    16530    fki_p_id_fkey    INDEX     H   CREATE INDEX fki_p_id_fkey ON public.game_publisher USING btree (p_id);
 !   DROP INDEX public.fki_p_id_fkey;
       public         postgres    false    207                       2606    16589    achievement ag_id_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.achievement
    ADD CONSTRAINT ag_id_fkey FOREIGN KEY (g_id) REFERENCES public.game(id) ON DELETE CASCADE NOT VALID;
 @   ALTER TABLE ONLY public.achievement DROP CONSTRAINT ag_id_fkey;
       public       postgres    false    199    2779    209                       2606    16732    cart c_g_id_fkey    FK CONSTRAINT     }   ALTER TABLE ONLY public.cart
    ADD CONSTRAINT c_g_id_fkey FOREIGN KEY (g_id) REFERENCES public.game(id) ON DELETE CASCADE;
 :   ALTER TABLE ONLY public.cart DROP CONSTRAINT c_g_id_fkey;
       public       postgres    false    199    2779    223                       2606    16737    cart c_u_id_fkey    FK CONSTRAINT        ALTER TABLE ONLY public.cart
    ADD CONSTRAINT c_u_id_fkey FOREIGN KEY (u_id) REFERENCES public."user"(id) ON DELETE CASCADE;
 :   ALTER TABLE ONLY public.cart DROP CONSTRAINT c_u_id_fkey;
       public       postgres    false    196    2775    223                       2606    16750    cart_inventory ci_i_id_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.cart_inventory
    ADD CONSTRAINT ci_i_id_fkey FOREIGN KEY (i_id) REFERENCES public.inventory(id) ON DELETE CASCADE;
 E   ALTER TABLE ONLY public.cart_inventory DROP CONSTRAINT ci_i_id_fkey;
       public       postgres    false    225    211    2801                       2606    16755    cart_inventory ci_u_id_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.cart_inventory
    ADD CONSTRAINT ci_u_id_fkey FOREIGN KEY (u_id) REFERENCES public."user"(id) ON DELETE CASCADE;
 E   ALTER TABLE ONLY public.cart_inventory DROP CONSTRAINT ci_u_id_fkey;
       public       postgres    false    2775    196    225                       2606    16599    game_developer gdd_id_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.game_developer
    ADD CONSTRAINT gdd_id_fkey FOREIGN KEY (d_id) REFERENCES public.developer(id) ON DELETE CASCADE NOT VALID;
 D   ALTER TABLE ONLY public.game_developer DROP CONSTRAINT gdd_id_fkey;
       public       postgres    false    205    201    2783                        2606    16594    game_developer gdg_id_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.game_developer
    ADD CONSTRAINT gdg_id_fkey FOREIGN KEY (g_id) REFERENCES public.game(id) ON DELETE CASCADE NOT VALID;
 D   ALTER TABLE ONLY public.game_developer DROP CONSTRAINT gdg_id_fkey;
       public       postgres    false    2779    199    205                       2606    16604    game_publisher gpg_id_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.game_publisher
    ADD CONSTRAINT gpg_id_fkey FOREIGN KEY (g_id) REFERENCES public.game(id) ON DELETE CASCADE NOT VALID;
 D   ALTER TABLE ONLY public.game_publisher DROP CONSTRAINT gpg_id_fkey;
       public       postgres    false    199    207    2779                       2606    16609    game_publisher gpp_id_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.game_publisher
    ADD CONSTRAINT gpp_id_fkey FOREIGN KEY (p_id) REFERENCES public.game_publisher(id) ON DELETE CASCADE NOT VALID;
 D   ALTER TABLE ONLY public.game_publisher DROP CONSTRAINT gpp_id_fkey;
       public       postgres    false    207    207    2795                       2606    16614    inventory ig_id_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.inventory
    ADD CONSTRAINT ig_id_fkey FOREIGN KEY (g_id) REFERENCES public.game(id) ON DELETE CASCADE NOT VALID;
 >   ALTER TABLE ONLY public.inventory DROP CONSTRAINT ig_id_fkey;
       public       postgres    false    2779    199    211                       2606    16696    library l_g_id_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.library
    ADD CONSTRAINT l_g_id_fkey FOREIGN KEY (g_id) REFERENCES public.game(id) ON DELETE CASCADE;
 =   ALTER TABLE ONLY public.library DROP CONSTRAINT l_g_id_fkey;
       public       postgres    false    2779    219    199                       2606    16701    library l_u_id_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.library
    ADD CONSTRAINT l_u_id_fkey FOREIGN KEY (u_id) REFERENCES public."user"(id) ON DELETE CASCADE;
 =   ALTER TABLE ONLY public.library DROP CONSTRAINT l_u_id_fkey;
       public       postgres    false    219    2775    196                       2606    16665    review r_g_id_fkey    FK CONSTRAINT        ALTER TABLE ONLY public.review
    ADD CONSTRAINT r_g_id_fkey FOREIGN KEY (g_id) REFERENCES public.game(id) ON DELETE CASCADE;
 <   ALTER TABLE ONLY public.review DROP CONSTRAINT r_g_id_fkey;
       public       postgres    false    199    2779    215                       2606    16649    user_achievement ua_a_id_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.user_achievement
    ADD CONSTRAINT ua_a_id_fkey FOREIGN KEY (a_id) REFERENCES public.achievement(id) ON DELETE CASCADE;
 G   ALTER TABLE ONLY public.user_achievement DROP CONSTRAINT ua_a_id_fkey;
       public       postgres    false    209    2797    213                       2606    16644    user_achievement ua_u_id_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.user_achievement
    ADD CONSTRAINT ua_u_id_fkey FOREIGN KEY (u_id) REFERENCES public."user"(id) ON DELETE CASCADE;
 G   ALTER TABLE ONLY public.user_achievement DROP CONSTRAINT ua_u_id_fkey;
       public       postgres    false    196    213    2775                       2606    16719    user_inventory ui_i_id_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.user_inventory
    ADD CONSTRAINT ui_i_id_fkey FOREIGN KEY (i_id) REFERENCES public.inventory(id) ON DELETE CASCADE;
 E   ALTER TABLE ONLY public.user_inventory DROP CONSTRAINT ui_i_id_fkey;
       public       postgres    false    2801    211    221                       2606    16714    user_inventory ui_u_id_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.user_inventory
    ADD CONSTRAINT ui_u_id_fkey FOREIGN KEY (u_id) REFERENCES public."user"(id) ON DELETE CASCADE;
 E   ALTER TABLE ONLY public.user_inventory DROP CONSTRAINT ui_u_id_fkey;
       public       postgres    false    2775    221    196            
           2606    16683    user_review ur_r_id_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.user_review
    ADD CONSTRAINT ur_r_id_fkey FOREIGN KEY (r_id) REFERENCES public.review(id) ON DELETE CASCADE;
 B   ALTER TABLE ONLY public.user_review DROP CONSTRAINT ur_r_id_fkey;
       public       postgres    false    217    2805    215            	           2606    16678    user_review ur_u_id_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.user_review
    ADD CONSTRAINT ur_u_id_fkey FOREIGN KEY (u_id) REFERENCES public."user"(id) ON DELETE CASCADE;
 B   ALTER TABLE ONLY public.user_review DROP CONSTRAINT ur_u_id_fkey;
       public       postgres    false    2775    196    217            �   �   x�}�=S�0�g�Wd�֋eZ��#wL96'��B1wvh�>��1֋_=z��C�.��HxK���S�s]��[��n���=5>�s��!����+�LG �,X2��c�Z�O����ph�Y
CiMDp�9��G�r%����<j�i��24���9��#AC��N>�i�g�ʗ��.�P5 ��K@�+���l�}�f�a�uO:�Fm�j�����zi�#��v�CmqS�{D�s�e�      �      x������ � �      �      x������ � �      �   �   x�-��J�@E�U_Q;7�y�X��!�F�75�r(��-���z#�]���[ةe��5m���@���僧�:'�qa��F�g�e�%4rdz��.d��['���� Ԇ�4�,�oE�wPsT�Ԧ��0��<z��Tş��� /��k��k�!�{Q�W.J��P�t�QK�O���'4�y���v2rF��6it*��f5�� �����"�a�K�      �   4  x��V�rE>���9*��֒�N���m�,�T�|펴�wg��Y9�	x�x.Tq^�y���g�����L�]������G]1~yy�����3��틛K�l$b\��\Ѹ�N�ޠ�T튯d�2�n���y�t:KmO��4X���vZ�@\��l��I���)�������=1:|yt�2I+�#=�F����i#kC��k��#UY��H�����\ZO�P�l�qjm6Y��)��ptdm�3��n�MZm�.���+q�Dwoo��C���o3'���pI�-�ȹ�f?��V�E����y�=��~����o�����7���C�䱸<~qzxq�����bs�)���.���q2=�&�&N��K�Е��G�}�����C�4�-�͔QN� И���|nmP���r%����Zm�V�*T�L�;tb���q���ɐ#�׶vF-����S4EA4ѯ��S��Nyl�����T��o�4���3s��%�S��.�!9х�联��be,��򀮧�9;���£mkO)N�E��),yih�5�����%/�*,vċ\��K���k����q��s�S�YOmF�B�q�R����(*%�+.�8�T�%�ʧ�B�����N����,�p�e�c$T�����@+l������X|�����x����:��������?��p��G�t����ǽe����l�@�[������6��g9f����f��0Q����c��Ri�AgZ��]2_FO�'[��v�䝡J�R��(�pg���*Ae�s�~�=h�	�?��6Yp2����:Í�4�!�z�
�ЙN��d��B���'�{M��K(��Ҁ/ÜJ�ys�v*�TW̛�q%���A�1:8�j7��	����l�ۢ�D4�H����Б��K�8W^�a���<MO��8�1C�C�ӡ��̖�tcnL������/i�P*����5I��r2И�Ѽ�yD:��h�u$Fw�!��6�MoA^��LP�fSk��f15몲.�>��.�[���Y��o��I�W���d	b�s�G�j��ښ�Ӵid/J����XFZ2�\g,*ǏC�v�������(.8V���ݶ�[^Q����b����R�T�!��;�kx���vb�K>������hLI�ӽu�w(�]2��.x�*��X�p�X�t�*_x�6�^�����Ƿ/&�ImҜ��i�U�4�<���v�tc�b�x�̓58 ӣ��� 7��}�f�C��z�Ia9Nv��!1�ƌ����<??��	���Ql���_a����
��T�w��7;����      �   b   x���DAþ�bVC����B�
`+1�I2��,m[����p��<��5�6�)-&��C�R�%V<q�qr�U����ʃ_�hP���~���{�      �   p   x�-�� г=LE~v��s�QR���hТ�:�`"�,$��̓���A�qiwPv����VX�ְҁ'|x=���'��74F���F�c>��i
^X��!���J      �   B   x�3�4��T�440�T�2�p�@u.c��S�2�4�L,N�bN333=C3K0�K$r��qqq N��      �      x�3�4 �4�=... a�      �   �   x�-�KkA��ݿ�o^dqռ��;�AXE^z�V��G����ĺUQE}�Қ�m�4u�9L{��:�)_�c�'�Wq��K92��g�Wk�F���*�!�8��Q���`�l�q���[z<k`��{<�|�2���ȯ�*V	��H� ?`ĲOx5�MX�EL�<�����̻��slRڃ�C�����CZ      �      x������ � �      �   �  x���ko�@�?�F��qm�F��Z/�&�A�K�0U����uM�������a�3 �K�������}ڑ! IB�v�`�ȡ�x�pS��yn��S�Y�BrG����B���"�����4���� �H�Rˎ5����� �4�Ҧ,�O��	X���1�0����2!:���^ϧ�t<�5�V�1�2���(Pw�Y�A�{��w4~;��m �$_b�����b�/�Ɣi#���
��R�ՍbH�2�%[Y'���w������9I�|�&�.-L^�dMz{\Y.ۘ7��� tpn���tP�-�)�(��L����������H����ų5�.[Ͱ[OJ]���rI�I^�ٕa�Yجnjن���b���*�d4<����,�:�WN>���y9�N�ѦE1����?���n���#)���
�z��@�n�:�$$�V5ݨ�҂I7��h�@Ҡ`7���0u��9�9�B�W��w�ɕ)���_��Hr�
i ������MH�Ȟ#��J� ��[�-�B��~S(U�$�pG�+�D����<j�p0h�t7��K�[���S�V����ݨ)d���@�=�sL����PE��ac`��Ao�)��nU	b!��DiG��	4Wp| �g��_&3s��1a��T�3����A�	;���N��m�I      �      x������ � �      �      x������ � �      �      x������ � �     